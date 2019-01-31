<?php

/**
 * @file
 * Provides search results as a Panels content type.
 */

$plugin = array(
  'title' => t('Ting search carousel'),
  'single' => TRUE,
  'description' => t('A carousel displaying search results against the data well'),
  'category' => t('Ting'),
);

/**
 * Return the pane contents.
 */
function obb_search_obb_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();

  

  return $block;
}

/**
 * Return the pane titles.
 */
function obb_search_obb_content_type_admin_title($subtype, $conf, $context) {
  return t('Obb search carousel');
}

/**
 * Returns an edit form for the content type.
 */
function obb_search_obb_carousel_content_type_edit_form($form, &$form_state) {
  return $form;
}
