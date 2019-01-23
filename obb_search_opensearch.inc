<?php

/* 
    Experimental module to speed up requests to the data well..
 */
function obb_search_carousel_get_results($query,$page=1,$number_results = 24,$onlyCovers = true)
{
    module_load_include('client.inc', 'ting');
    $options = array();
    $options['enrich'] = FALSE;
    $options['collectionType'] = 'manifestation';
    $entities_found_with_covers = array();
    
    $max_pages_to_crawl = 20;
    $crawled_pages = 0;
    $stop_page = 0;
    $search_result = null;
    $stop = false;
    do{
        $search_result = ting_do_search(oc_query_parser($query), $page + $crawled_pages , $number_results, $options);

        $collections = $search_result->collections;
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
        $stop_page = $page +$crawled_pages;
    }while($stop == false && $search_result->more != 0 && $crawled_pages <= $max_pages_to_crawl);
    
    return array('entities' => $entities_found_with_covers,'stoppage' => $stop_page,'hasmore' => $search_result->more);
}
