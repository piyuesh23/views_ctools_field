<?php
// $Id: views_ctools_field_handler_field_markup.inc,v 1.1.2.5 2008/11/19 13:53:47 casey Exp $

/**
 * @file
 * Contains the 'ctools_field' markup field handler.
 */

/**
 * Field handler to display custom markup text.
 */
class views_ctools_field_handler_field_markup extends views_handler_field {
  
  function query() {
    $this->field_alias = 'ctools_field_markup_'. $this->position;
    
    $value = db_escape_string($this->options['value']);
    $this->query->add_field('', "'$value'", $this->field_alias);
  }
  
  function option_definition() {
    $options = parent::option_definition();
    $options['views_ctools_field'] = array('default' => '');
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    ctools_include('plugins');
    ctools_include('context');
    ctools_include('content');
    
    $plugin = $this->options['field'];
    
    $plugin_id = $this->options['field'];
    $plugin = ctools_get_plugins('ctools','content_types', $this->options['field'], $plugin_id);
    
    $edit_form_callback = $plugin['module'].'_'.$plugin_id.'_content_type_edit_form';
    if (function_exists($edit_form_callback)) {
      $form['views_ctools_field'] = array(
        '#type' => 'fieldset', 
        '#title' => t('Ctools Field Settings'), 
        '#collapsible' => FALSE, 
        '#collapsed' => FALSE,
        '#tree' => TRUE, 
      );
      $sub_form_state = array('conf' => $this->getCtoolsContentTypeConf());
      $edit_form_callback($form['views_ctools_field'], $sub_form_state);
    }
    
    return $form;
  }
  
  function render($values) {
    ctools_include('plugins');
    ctools_include('context');
    ctools_include('content');
    
    $plugin_id = $this->options['field'];
    $plugin = ctools_get_plugins('ctools','content_types', $this->options['field'], $plugin_id);
    
    $conf = $this->getCtoolsContentTypeConf();
    
    //@@TODO: Do this programatically, Allow for selection of node based on relationship / base-table
    // Also allow for user and term contexts via relationships....
    if ($values->nid) {
      $node = node_load($values->nid);
      $node_context = ctools_context_create('node', $node);
      $node_context->identifier = t('This node');
      $node_context->keyword = 'node';
      
      $context = array('node' => $node_context);
      $conf['context'] = 'node';
      
      $ctools_content = ctools_content_render($plugin_id, $plugin_id, $conf, array(), array(), $context);
    }
    else {
      $context = array();
      $ctools_content = ctools_content_render($plugin_id, $plugin_id, $conf);
    }
    
    return $ctools_content->content;
  }
  
  function getCtoolsContentTypeConf() {
    if (empty($this->options['views_ctools_field'])) {
      return array();
    }
    
    ctools_include('plugins');
    ctools_include('context');
    ctools_include('content');
    
    $plugin_id = $this->options['field'];
    $plugin = ctools_get_plugins('ctools','content_types', $this->options['field'], $plugin_id);
    
    $edit_submit_callback = $plugin['module'].'_'.$plugin_id.'_content_type_edit_form_submit';
    
    // If there is no callback, pass back the raw form values
    if (!function_exists($edit_submit_callback)) {
      return $this->options['views_ctools_field'];
    }
    
    $form = array();
    $form_state = array(
      'conf' => array(),
      'values' => $this->options['views_ctools_field'],
      'plugin' => $plugin, 
    );
    
    $edit_submit_callback($form, $form_state);
    
    return $form_state['conf'];
  }
  
}