<?php

/* 
    Experimental module to speed up requests to the data well..
 */
function obb_search_carousel_get_results($query,$page=1,$start_index = 0,$number_results = 48,$onlyCovers = true)
{
    module_load_include('client.inc', 'ting');
    $options = array();
    $options['enrich'] = FALSE;
    $options['collectionType'] = 'manifestation';
    $entities_found_with_covers = array();
    
    $results_pr_page =50; // 50 is max.
    $max_pages_to_crawl = 20;
    $max_pages_no_covers = 15; // sometimes there are just no more covers ?!
    $crawled_pages = 0;
    $crawled_pages_no_covers = 0;
    $stop_page = 0;
    $search_result = null;
    $stop = false;
    $stop_index = 0;
    $entities = null;
    $last_mat_index = 0;
    do{
        $entities = array();
        $search_result = ting_do_search(oc_query_parser($query), $page + $crawled_pages , $results_pr_page, $options);
        $collections = $search_result->collections;
        
        /*
         * unfold collections to single objects.
         * cache ? 
         */
        foreach ($collections as $collection) {
          foreach ($collection->reply->objects as $entity) {
            $entity_id = $entity->id;
            $entities[$entity_id] = $entity;//why load objects we are not gonna show ? //ding_entity_load($entity_id, 'ting_object');
          }
        }
        if($start_index != 0)
        {
            $entities = array_slice($entities,$start_index);
            
        }
        $covers = ting_covers_get(array_keys($entities));
        // Loop over the fetched covers and build items.
        if($onlyCovers && sizeof($covers) != 0)
        {
            $crawled_pages_no_covers = 0;
            foreach ($covers as $id => $path) {
                //$entities_found_with_covers[]['#object'] = ding_entity_load($id,'ting_object'); //can we make it faster ?
                $new_entity = new TingEntity();
                $new_entity->reply = $entities[$id];
                $new_entity->ding_entity_id = $id;
                $new_entity->localId = $entities[$id]->localId;
                
                $entities_found_with_covers[]['#object'] = $new_entity;
                unset($new_entity); //cleanup
                if(sizeof($entities_found_with_covers) == $number_results)
                {
                    $last_mat_index = obb_search_carousel_last_material_index($entities,$id);
                    if($start_index != 0)
                    {
                        $last_mat_index += $start_index; //we are still searching the same page.
                    }
                    $stop = true;
                    break;
                }
            }
        }
        else
        {
            $crawled_pages_no_covers++;
            if($crawled_pages_no_covers == $max_pages_no_covers)
            {
                $search_result->more = 0; // so many pages no covers...we are done!
            }
        }
        $stop_page = $page + $crawled_pages; 
        $crawled_pages++;
        $start_index = 0; //we are switching pages!
    }while($stop == false && $search_result->more != 0 ); //&& $crawled_pages <= $max_pages_to_crawl); No Limits
    
    return array('entities' => $entities_found_with_covers,'extra' => array('page' => $stop_page,'hasmore' => $search_result->more,'start_index' =>  $last_mat_index,'onlycovers' => $onlyCovers));
}
function obb_search_carousel_last_material_index($entities,$last_id)
{
    $index = 0;
    foreach($entities as $key => $entity)
    {
        if($key == $last_id)
        {
            return $index+1; //dont include current target.
        }
        $index++;
    }
    return 0;
}