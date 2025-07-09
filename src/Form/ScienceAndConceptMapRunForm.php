<?php

/**
 * @file
 * Contains \Drupal\science_and_concept_map\Form\ScienceAndConceptMapRunForm.
 */

namespace Drupal\science_and_concept_map\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class ScienceAndConceptMapRunForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'science_and_concept_map_run_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $options_first = _list_of_science_and_concept_map();
    $url_science_and_concept_map_id = (int) arg(2);
    $science_and_concept_map_data = _science_and_concept_map_information($url_science_and_concept_map_id);
    if ($science_and_concept_map_data == 'Not found') {
      $url_science_and_concept_map_id = '';
    } //$science_and_concept_map_data == 'Not found'
    if (!$url_science_and_concept_map_id) {
      $selected = !$form_state->getValue(['science_and_concept_map']) ? $form_state->getValue(['science_and_concept_map']) : key($options_first);
    } //!$url_science_and_concept_map_id
    elseif ($url_science_and_concept_map_id == '') {
      $selected = 0;
    } //$url_science_and_concept_map_id == ''
    else {
      $selected = $url_science_and_concept_map_id;
    }
    $form = [];
    $form['science_and_concept_map'] = [
      '#type' => 'select',
      '#title' => t('Title of the science and concept map'),
      '#options' => _list_of_science_and_concept_map(),
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => 'science_and_concept_map_project_details_callback'
        ],
    ];
    if (!$url_science_and_concept_map_id) {
      $form['science_and_concept_map_details'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_science_and_concept_map_details"></div>',
      ];
      $form['selected_science_and_concept_map'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_selected_science_and_concept_map"></div>',
      ];
    } //!$url_science_and_concept_map_id
    else {
      $science_and_concept_map_default_value = $url_science_and_concept_map_id;
      $form['science_and_concept_map_details'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_science_and_concept_map_details">' . _science_and_concept_map_details($science_and_concept_map_default_value) . '</div>',
      ];
      // @FIXME
      // l() expects a Url object, created from a route name or external URI.
      // $form['selected_science_and_concept_map'] = array(
      // 			'#type' => 'item',
      // 			'#markup' => '<div id="ajax_selected_science_and_concept_map">' . l('Download Abstract', "science-and-concept-map-project/download/abstract-file/" . $science_and_concept_map_default_value) . '<br>' . l('Download science and concept map', 'science-and-concept-map-project/full-download/project/' . $science_and_concept_map_default_value) . '</div>'
      // 		);

    }
    return $form;
  }
  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state){
  }
}
?>
