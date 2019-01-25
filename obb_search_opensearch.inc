<?php

/* 
    Experimental module to speed up requests to the data well..
 */
function obb_search_carousel_get_results($query,$page=1,$start_index = 0,$number_results = 24,$onlyCovers = true)
{
    module_load_include('client.inc', 'ting');
    $options = array();
    $options['enrich'] = FALSE;
    $options['collectionType'] = 'manifestation';
    $entities_found_with_covers = array();
    
    $results_pr_page =24;
    $max_pages_to_crawl = 20;
    $crawled_pages = 0;
    $stop_page = 0;
    $search_result = null;
    $stop = false;
    $stop_index = 0;
    $entities = null;
    do{
        $entities = array();
        $search_result = ting_do_search(oc_query_parser($query), $page + $crawled_pages , $results_pr_page, $options);
        $collections = $search_result->collections;
        if($start_index != 0)
        {
            $collections = array_slice($collections,$start_index);
            $start_index = 0;
        }
        /*
         * Why not load multiple ?
         */
        foreach ($collections as $collection) {
          foreach ($collection->reply->objects as $entity) {
            $entity_id = $entity->id;
            $entities[$entity_id] = $entity_id;//why load objects we are not gonna show ? //ding_entity_load($entity_id, 'ting_object');
          }
        }
        $covers = ting_covers_get(array_keys($entities));
        // Loop over the fetched covers and build items.
        if($onlyCovers)
        {
            foreach ($covers as $id => $path) {
                $entities_found_with_covers[$id] = ding_entity_load($entities[$id],'ting_object'); //can we make it faster ?
                if(sizeof($entities_found_with_covers) == $number_results)
                {
                    $stop = true;
                    break;
                }
            }
        }
        $crawled_pages++;
        $stop_page = $page + $crawled_pages;
        
        
    }while($stop == false && $search_result->more != 0 && $crawled_pages <= $max_pages_to_crawl);
    $cover_keys = array_keys($entities_found_with_covers);
    $last_found_cover = end($cover_keys);
    $last_mat_index = obb_search_carousel_last_material_index($entities,$last_found_cover);
    return array('entities' => $entities_found_with_covers,'extra' => array('page' => $stop_page,'hasmore' => $search_result->more,'start_index' =>  $last_mat_index,'onlycovers' => $onlyCovers));
}
function obb_search_carousel_last_material_index($entities,$last_id)
{
    $index = 0;
    foreach($entities as $entity)
    {
        if($entity == $last_id)
        {
            return $index;
        }
        $index++;
    }
    return 0;
}