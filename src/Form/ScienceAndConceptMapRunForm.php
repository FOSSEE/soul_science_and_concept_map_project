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
    // Try to capture the selected project ID from path or query.
    $request = \Drupal::request();
    $url_science_and_concept_map_id = (int) (function() use ($request) {
      $from_query = (int) ($request->query->get('id') ?? 0);
      if ($from_query) { return $from_query; }
      // Legacy fallback: extract 3rd arg segment if present.
      if (function_exists('arg')) { return (int) arg(2); }
      return 0;
    })();
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
      // Selected project actions: download abstract and full project.
      $abstract_url = \Drupal\Core\Url::fromRoute('science_and_concept_map.download_upload_file', [
        'proposal_id' => $science_and_concept_map_default_value,
      ]);
      $abstract_link = \Drupal\Core\Link::fromTextAndUrl(t('Download Abstract'), $abstract_url)->toString();

      $full_url = \Drupal\Core\Url::fromRoute('science_and_concept_map.download_full_project', [
        'id' => $science_and_concept_map_default_value,
      ]);
      $full_link = \Drupal\Core\Link::fromTextAndUrl(t('Download science and concept map'), $full_url)->toString();

      $form['selected_science_and_concept_map'] = [
        '#type' => 'item',
        '#markup' => '<div id="ajax_selected_science_and_concept_map">' . $abstract_link . '<br>' . $full_link . '</div>',
      ];

    }
    return $form;
  }
  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state){
  }
}
?>
