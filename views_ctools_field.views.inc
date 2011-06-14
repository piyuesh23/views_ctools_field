<?php
// $Id: views_ctools_field.views.inc,v 1.1.2.5 2009/01/09 18:32:33 casey Exp $

/**
 * Implementation of hook_views_handlers().
 *
 * @return array
 */
function views_ctools_field_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_ctools_field') .'/includes',
    ),
    'handlers' => array(
      'views_ctools_field_handler_field_markup' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 *
 * @return array
 */
function views_ctools_field_views_data() {
  //@@TODO, create the fields progamatically, depending on the available base tables (node, term, user).
  // keep the #global join for non-context ctools content types
  
  $data['ctools_field']['table']['group'] = t('Chaos Tools Field');
  $data['ctools_field']['table']['join'] = array(
    '#global' => array(), 
  );
  
  ctools_include('plugins');
  ctools_include('context');
  $plugins = ctools_get_plugins('ctools','content_types');
  
  foreach ($plugins as $plugin_id => $plugin) {
    if ($plugin['required context']) {
      if ($plugin['required context']->keywords != 'node') continue;
    }
    $data['ctools_field'][$plugin_id] = array (
      'title' => $plugin['title'],
      'field' => array(
        'handler' => 'views_ctools_field_handler_field_markup',
        'click sortable' => FALSE,
        'notafield' => TRUE,
      ),
    );
    if (isset($plugin['description'])) {
      $data['ctools_field'][$plugin_id]['help'] = $plugin['description'];
    }
  }
  
  return $data;
}