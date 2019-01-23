<?php
/**
 * @file
 * obb_search_carousel_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function obb_search_carousel_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function obb_search_carousel_feature_node_info() {
  $items = array(
    'obb_search_carousel' => array(
      'name' => t('obb search carousel'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
