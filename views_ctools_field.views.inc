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
  ctools_include('plugins');
  ctools_include('context');
  
  $data = array();
  
  // Psuedo table for global content type that have no required context
  $data['ctools_field_global']['table']['group'] = t('Chaos Tools Field');
  $data['ctools_field_global']['table']['join'] = array(
    '#global' => array(), 
  );
  
  // Psudo table for content-types that have a node as required context
  $data['ctools_field_node']['table']['group']  = t('Chaos Tools Field - Node');  
  $data['ctools_field_node']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Node'),
    'help' => t('ctools content with a node context'),
  );
  $data['ctools_field_node']['table']['join'] = array(
      'node' => array(
        'left_table' => 'node',
        'left_field' => 'nid',
        'field' => 'nid',
        'table' => 'node',
      ),
  );

  $plugins = ctools_get_plugins('ctools','content_types');
  
  // For each plugin, define a 'column' on a psudo-table
  foreach ($plugins as $plugin_id => $plugin) {
    // Determine the base table based on context
    if ($plugin['required context']) {
      if ($plugin['required context']->keywords == 'node') {
        $base_table = 'ctools_field_node';
        $real_field = 'nid';  
      }
      if ($plugin['required context']->keywords != 'node') continue;  
    }
    else {
      $base_table = 'ctools_field_global';
      $real_field = '';
    }
    
    $data[$base_table][$plugin_id] = array (
      'title' => $plugin['title'],
      'field' => array(
        'handler' => 'views_ctools_field_handler_field_markup',
        'click sortable' => FALSE,
      ), 
    );
    if (isset($plugin['description'])) {
      $data[$base_table][$plugin_id]['help'] = $plugin['description'];
    }
    
    // The the 'real field' so that these fields actually point at the entity id (nid, uid, and tid)
    $data[$base_table][$plugin_id]['real field'] = $real_field;
    #if ($real_field) {
    #  $data[$base_table][$plugin_id]['real field'] = $real_field;
    #}
    #else {
    #  $data[$base_table][$plugin_id]['field']['notafield'] = TRUE;
    #}
    
  }
  
  dpm($data);
  
  return $data;
}