<?php
// $Id$
function science_and_concept_map_settings_form($form, $form_state)
{
	$form['emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(Bcc) Notification emails'),
		'#description' => t('Specify emails id for Bcc option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails')
	);
	$form['cc_emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(Cc) Notification emails'),
		'#description' => t('Specify emails id for Cc option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails')
	);
	$form['from_email'] = array(
		'#type' => 'textfield',
		'#title' => t('Outgoing from email address'),
		'#description' => t('Email address to be display in the from field of all outgoing messages'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email')
	);
	$form['extensions']['abstract_upload'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed file extensions for uploading abstract file'),
		'#description' => t('A comma separated list WITHOUT SPACE of source file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_abstract_upload_extensions')
	);
	$form['extensions']['report_upload'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed file extensions for report'),
		'#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_report_upload_extensions')
	);
	$form['extensions']['science_and_concept_map_code_upload'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed extensions for code files'),
		'#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_code_files_extensions')
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function science_and_concept_map_settings_form_validate($form, &$form_state)
{
	return;
}
function science_and_concept_map_settings_form_submit($form, &$form_state)
{
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_emails', $form_state['values']['emails'])->save();
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_cc_emails', $form_state['values']['cc_emails'])->save();
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_from_email', $form_state['values']['from_email'])->save();
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_abstract_upload_extensions', $form_state['values']['abstract_upload'])->save();
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_report_upload_extensions', $form_state['values']['report_upload'])->save();
	\Drupal::configFactory()->getEditable('science_and_concept_map.settings')->set('science_and_concept_map_code_files_extensions', $form_state['values']['science_and_concept_map_code_upload'])->save();
	\Drupal::messenger()->addStatus(t('Settings updated'));
}