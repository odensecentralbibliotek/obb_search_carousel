<?php
function obb_search_carousel_settings_admin_form() {
    $form = array();
     $form['obb_search_carousel_preload_page_count'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('obb_search_carousel_preload_page_count', 6),
      '#title' => t('preloadede materiale sider'),
      '#attributes' => array(
          'placeholder' => t('preloadede materiale sider '),
      ),
      '#description' => t('preloadede materiale sider'),
     );
    return system_settings_form($form);
}


