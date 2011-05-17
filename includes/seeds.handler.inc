<?php
/**
 * @file
 * Defines the SeedsNodeHandler class.
 */

class SeedsBeanHandler implements SeedHandler {
  public function __construct($key, $seed) {
    $this->key = $key;
    $this->seed = $seed;
  }

  /**
   * Load the seed data from the file specified in the data_file property.
   */
  public function load_data() {
    $data = seeds_load_data_from_file($this->seed['data_file'], 'bean');
    drupal_alter('seeds_bean_import', $data);
//    $data = self::strip_data($data);

    return $data;
  }

  /**
   * Load the object from the database if it exists.
   */
  public function load_object() {
    $bid = seeds_get_serial_id($this->key);
    if (!empty($bid) && ($bean = bean_load($bid))) {
      return $bean;
    }
    return FALSE;
  }

  /**
   * 
   */
  public function object_path() {
    $bid = seeds_get_serial_id($this->key);
    if (!empty($bid)) {
      return 'admin/structure/bean/block/'. $bid;
    }
    return FALSE;
  }

  /**
   * Strip any unnecessary properties from the data array. Used by both import
   * and export for consistency.
   */
  static public function strip_data($data) {
    $keys = array('bid', 'rdf_mapping');
    foreach ($keys as $key) {
      unset($data[$key]);
    }

    return $data;
  }

  /**
   * Import the seed into the database.
   */
  public function import() {
    $json = $this->load_data();
    $bean = $this->load_object();

    if (empty($bean)) {
      $bean = entity_import('bean', $json);
    }
    else {
      $data = drupal_json_decode($json);
      foreach ($data as $key => $value) {
        $bean->$key = $value;
      }
    }

    bean_save($bean);
    seeds_set_serial_id($this->key, $bean->bid);
    return $bean;
  }

  /**
   * Export the specified node.
   */
  static public function export($bean) {
    drupal_alter('seeds_bean_export', $bean);

    unset($bean->bid);
    return entity_export('bean', $bean);
  }

}
