<?php
// $Id$
function science_and_concept_map_download_full_project()
{
	$user = \Drupal::currentUser();
	$id = arg(3);
	$root_path = science_and_concept_map_path();
	//var_dump($root_path);die;
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$science_and_concept_map_q = $query->execute();
	$science_and_concept_map_data = $science_and_concept_map_q->fetchObject();
	$SCIENCEANDCONCEPTMAP = $science_and_concept_map_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$science_and_concept_map_udc_q = $query->execute();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$query = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts_file');
	$query->fields('soul_science_and_concept_map_submitted_abstracts_file');
	$query->condition('proposal_id', $id);
	$project_files = $query->execute();
	while ($soul_project_files = $project_files->fetchObject()) {
		$zip->addFile($root_path . $SCIENCEANDCONCEPTMAP . 'project_files/' . $soul_project_files->filepath, $SCIENCEANDCONCEPTMAP . str_replace(' ', '_', basename($soul_project_files->filename)));
	}
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $science_and_concept_map_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_clean();
			readfile($zip_filename);
			unlink($zip_filename);
			/*flush();
			ob_end_flush();
			ob_clean();*/
			
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $science_and_concept_map_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			//ob_end_flush();
			ob_clean();
			//flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		\Drupal::messenger()->addError("There are no science and concept map project in this proposal to download");
		drupal_goto('science-and-concept-map-project/full-download/project');
	}
}
function science_and_concept_map_download_completed_project()
{
		$user = \Drupal::currentUser();
	$id = arg(3);
	$root_path = science_and_concept_map_path();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$science_and_concept_map_q = $query->execute();
	$science_and_concept_map_data = $science_and_concept_map_q->fetchObject();
	$SCIENCEANDCONCEPTMAP = $science_and_concept_map_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$science_and_concept_map_udc_q = $query->execute();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $id);
	$query = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts_file');
	$query->fields('soul_science_and_concept_map_submitted_abstracts_file');
	$query->condition('proposal_id', $id);
	$project_files = $query->execute();
	//var_dump($root_path . $SCIENCEANDCONCEPTMAP . 'project_files/');die;
	while ($soul_project_files = $project_files->fetchObject()) {
		$zip->addFile($root_path . $SCIENCEANDCONCEPTMAP . 'project_files/' . $soul_project_files->filepath, $SCIENCEANDCONCEPTMAP . str_replace(' ', '_', basename($soul_project_files->filename)));
	}
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $science_and_concept_map_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			//ob_end_flush();
			ob_clean();
			//flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $science_and_concept_map_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			//ob_end_flush();
			ob_clean();
			//flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		\Drupal::messenger()->addError("There are science and concept map project in this proposal to download");
		drupal_goto('science-and-concept-map-project/full-download/project');
	}
}