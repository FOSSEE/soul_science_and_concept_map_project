<?php
// $Id$
function science_and_concept_map_download_upload_file()
{
	$proposal_id = arg(3);
	$root_path = science_and_concept_map_document_path();
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$query->range(0, 1);
	$result = $query->execute();
	$science_and_concept_map_upload_file = $result->fetchObject();
	$samplecodename = substr($science_and_concept_map_upload_file->abstractfilepath, strrpos($science_and_concept_map_upload_file->abstractfilepath, '/') + 1);
			ob_clean();
            header("Pragma: public");
            header("Expires: 0");
            header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
            header("Cache-Control: public");
            header("Content-Description: File Transfer");
            header('Content-Type: application/pdf');
            header('Content-disposition: attachment; filename="' . $samplecodename . '"');
            header('Content-Length: ' . filesize($root_path . $science_and_concept_map_upload_file->abstractfilepath));
            header("Content-Transfer-Encoding: binary");
            header('Expires: 0');
            header('Pragma: no-cache');
            ob_clean();
            readfile($root_path . $science_and_concept_map_upload_file->abstractfilepath);
	        //ob_end_flush();
           
           //flush();
}
function soul_science_and_concept_map_project_files() {
	$proposal_id = arg(3);
	$root_path = science_and_concept_map_document_path();
	$query = db_select('soul_science_and_concept_map_submitted_abstracts_file');
	$query->fields('soul_science_and_concept_map_submitted_abstracts_file');
	$query->condition('proposal_id', $proposal_id);
	$result = $query->execute();
	$soul_science_and_concept_map_project_files = $result->fetchObject();
	//var_dump($soul_science_and_concept_map_project_files);die;
	$query1 = db_select('soul_science_and_concept_map_proposal');
	$query1->fields('soul_science_and_concept_map_proposal');
	$query1->condition('id', $proposal_id);
	$result1 = $query1->execute();
	$science_and_concept_map = $result1->fetchObject();
	$directory_name = $science_and_concept_map->directory_name . '/project_files/';
	$samplecodename = $soul_science_and_concept_map_project_files->filename;
	ob_clean();
	header("Pragma: public");
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
	header("Cache-Control: public");
	header("Content-Description: File Transfer");
	header("Content-Type: application/pdf");
	header('Content-disposition: attachment; filename="' . $samplecodename . '"');
	header("Content-Length: " . filesize($root_path . $directory_name . $samplecodename));
	header("Content-Transfer-Encoding: binary");
	header("Expires: 0");
	header("Pragma: no-cache");
	ob_clean();
	readfile($root_path . $directory_name . $samplecodename);
	//ob_end_flush();
	//ob_clean();
}
/*
function science_and_concept_map_download_solution_file()
{
	$solution_file_id = arg(3);
	$root_path = science_and_concept_map_path();
	// $solution_files_q = db_query("SELECT * FROM {science_and_concept_map_solution_files} WHERE id = %d LIMIT 1", $solution_file_id);
	$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM soul_science_and_concept_map_solution_files lmsf JOIN soul_science_and_concept_map_solution lms JOIN soul_science_and_concept_map_experiment lme JOIN soul_science_and_concept_map_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id LIMIT 1", array(
		':solution_id' => $solution_file_id
	));
	$solution_file_data = $solution_files_q->fetchObject();
	header('Content-Type: ' . $solution_file_data->filemime);
	//header('Content-Type: application/octet-stram');
	header('Content-disposition: attachment; filename="' . str_replace(' ', '_', ($solution_file_data->filename)) . '"');
	header('Content-Length: ' . filesize($root_path . $solution_file_data->directory_name . '/' . $solution_file_data->filepath));
	readfile($root_path . $solution_file_data->directory_name . '/' . $solution_file_data->filepath);
}
function science_and_concept_map_download_dependency_file()
{
	$dependency_file_id = arg(3);
	$root_path = science_and_concept_map_path();
	//$dependency_file_q = db_query("SELECT * FROM {science_and_concept_map_dependency_files} WHERE id = %d LIMIT 1", $dependency_file_id);
	$query = db_select('soul_science_and_concept_map_dependency_files');
	$query->fields('soul_science_and_concept_map_dependency_files');
	$query->condition('id', $dependency_file_id);
	$query->range(0, 1);
	$dependency_files_q = $query->execute();
	$dependency_file_data = $dependency_files_q->fetchObject();
	header('Content-Type: ' . $dependency_file_data->filemime);
	header('Content-disposition: attachment; filename="' . str_replace(' ', '_', ($dependency_file_data->filename)) . '"');
	header('Content-Length: ' . filesize($root_path . $dependency_file_data->filepath));
	readfile($root_path . $dependency_file_data->filepath);
}
function science_and_concept_map_download_solution()
{
	$solution_id = arg(3);
	$root_path = science_and_concept_map_path();
	$query = db_select('soul_science_and_concept_map_solution');
	$query->fields('soul_science_and_concept_map_solution');
	$query->condition('id', $solution_id);
	$solution_q = $query->execute();
	$solution_data = $solution_q->fetchObject();
	$query = db_select('soul_science_and_concept_map_experiment');
	$query->fields('soul_science_and_concept_map_experiment');
	$query->condition('id', $solution_data->experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM soul_science_and_concept_map_solution_files lmsf JOIN soul_science_and_concept_map_solution lms JOIN soul_science_and_concept_map_experiment lme JOIN soul_science_and_concept_map_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(
		':solution_id' => $solution_id
	));
	$query = db_select('soul_soul_science_and_concept_map_solution_dependency');
	$query->fields('soul_science_and_concept_map_solution_dependency');
	$query->condition('solution_id', $solution_id);
	$solution_dependency_files_q = $query->execute();
	$CODE_PATH = 'CODE' . $solution_data->code_number . '/';
	/* zip filename */
	//$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	/*$zip = new ZipArchive;
	$zip->open($zip_filename, ZipArchive::CREATE);
	while ($solution_files_row = $solution_files_q->fetchObject())
	{
		$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
	} //$solution_files_row = $solution_files_q->fetchObject()
	/* dependency files */
	/*while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
	{
		$query = db_select('soul_science_and_concept_map_dependency_files');
		$query->fields('soul_science_and_concept_map_dependency_files');
		$query->condition('id', $solution_dependency_files_row->dependency_id);
		$query->range(0, 1);
		$dependency_file_data = $query->execute()->fetchObject();
		if ($dependency_file_data)
			$zip->addFile($root_path . $dependency_file_data->filepath, $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
	} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		/* download zip file */
		/*header('Content-Type: application/zip');
		header('Content-disposition: attachment; filename="CODE' . $solution_data->code_number . '.zip"');
		header('Content-Length: ' . filesize($zip_filename));
		ob_clean();
		//flush();
		readfile($zip_filename);
		unlink($zip_filename);
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no files in this solutions to download", 'error');
		drupal_goto('science-and-concept-map-project/science-and-concept-map-project-run');
	}
}
function science_and_concept_map_download_experiment()
{
	$experiment_id = (int) arg(3);
	$root_path = science_and_concept_map_path();
	/* get solution data */
/*	$query = db_select('soul_science_and_concept_map_experiment');
	$query->fields('soul_science_and_concept_map_experiment');
	$query->condition('id', $experiment_id);
	$experiment_q = $query->execute();
	$experiment_data = $experiment_q->fetchObject();
	$EXP_PATH = 'EXP' . $experiment_data->number . '/';
	/* zip filename */
	//$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	/*$zip = new ZipArchive;
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('soul_science_and_concept_map_solution');
	$query->fields('soul_science_and_concept_map_solution');
	$query->condition('experiment_id', $experiment_id);
	$query->condition('approval_status', 1);
	$solution_q = $query->execute();
	while ($solution_row = $solution_q->fetchObject())
	{
		$CODE_PATH = 'CODE' . $solution_row->code_number . '/';
		$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM soul_science_and_concept_map_solution_files lmsf JOIN soul_science_and_concept_map_solution lms JOIN soul_science_and_concept_map_experiment lme JOIN soul_science_and_concept_map_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.solution_id = :solution_id", array(
			':solution_id' => $solution_row->id
		));
		while ($solution_files_row = $solution_files_q->fetchObject())
		{
			$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $EXP_PATH . $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
		} //$solution_files_row = $solution_files_q->fetchObject()
		/* dependency files */
	/*	$query = db_select('soul_science_and_concept_map_solution_dependency');
		$query->fields('soul_science_and_concept_map_solution_dependency');
		$query->condition('solution_id', $solution_row->id);
		$solution_dependency_files_q = $query->execute();
		while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
		{
			$query = db_select('soul_science_and_concept_map_dependency_files');
			$query->fields('soul_science_and_concept_map_dependency_files');
			$query->condition('id', $solution_dependency_files_row->dependency_id);
			$query->range(0, 1);
			$dependency_file_data = $query->execute()->fetchObject();
			if ($dependency_file_data)
				$zip->addFile($root_path . $dependency_file_data->filepath, $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
		} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
	} //$solution_row = $solution_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		/* download zip file */
	/*	header('Content-Type: application/zip');
		header('Content-disposition: attachment; filename="EXP' . $experiment_data->number . '.zip"');
		header('Content-Length: ' . filesize($zip_filename));
		ob_clean();
		//flush();
		readfile($zip_filename);
		unlink($zip_filename);
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no solutions in this experiment to download", 'error');
		drupal_goto('science-and-concept-map-project/science-and-concept-map-project-run');
	}
}
function science_and_concept_map_download_project()
{
	global $user;
	$lab_id = arg(3);
	$root_path = science_and_concept_map_path();
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $lab_id);
	$lab_q = $query->execute();
	$lab_data = $lab_q->fetchObject();
	$LAB_PATH = $lab_data->lab_title . '/';
	/* zip filename */
//	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
/*	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('soul_science_and_concept_map_experiment');
	$query->fields('soul_science_and_concept_map_experiment');
	$query->condition('proposal_id', $lab_id);
	$experiment_q = $query->execute();
	while ($experiment_row = $experiment_q->fetchObject())
	{
		$EXP_PATH = 'EXP' . $experiment_row->number . '/';
		//$solution_q = db_query("SELECT * FROM {science_and_concept_map_solution} WHERE experiment_id = %d AND approval_status = 1", $experiment_row->id);
		$query = db_select('soul_science_and_concept_map_solution');
		$query->fields('soul_science_and_concept_map_solution');
		$query->condition('experiment_id', $experiment_row->id);
		$query->condition('approval_status', 1);
		$solution_q = $query->execute();
		while ($solution_row = $solution_q->fetchObject())
		{
			$CODE_PATH = 'CODE' . $solution_row->code_number . '/';
			//$solution_files_q = db_query("SELECT * FROM {science_and_concept_map_solution_files} WHERE solution_id = %d", $solution_row->id);
			$solution_files_q = db_query("SELECT lmsf.*, lmp.directory_name FROM soul_science_and_concept_map_solution_files lmsf JOIN soul_science_and_concept_map_solution lms JOIN soul_science_and_concept_map_experiment lme JOIN soul_science_and_concept_map_proposal lmp WHERE lms.id = lmsf.solution_id AND lme.id = lms.experiment_id AND lmp.id = lme.proposal_id AND lmsf.id = :solution_id", array(
				':solution_id' => $solution_row->id
			));
			$query = db_select('soul_science_and_concept_map_solution_dependency');
			$query->fields('soul_science_and_concept_map_solution_dependency');
			$query->condition('solution_id', $solution_row->id);
			$solution_dependency_files_q = $query->execute();
			while ($solution_files_row = $solution_files_q->fetchObject())
			{
				$zip->addFile($root_path . $solution_files_row->directory_name . '/' . $solution_files_row->filepath, $EXP_PATH . $CODE_PATH . str_replace(' ', '_', ($solution_files_row->filename)));
				//var_dump($zip->numFiles);
			} //$solution_files_row = $solution_files_q->fetchObject()
			// die;
			/* dependency files */
	/*		while ($solution_dependency_files_row = $solution_dependency_files_q->fetchObject())
			{
				//$dependency_file_data = (db_query("SELECT * FROM {science_and_concept_map_dependency_files} WHERE id = %d LIMIT 1", $solution_dependency_files_row->dependency_id))->fetchObject();
				$query = db_select('soul_science_and_concept_map_dependency_files');
				$query->fields('soul_science_and_concept_map_dependency_files');
				$query->condition('id', $solution_dependency_files_row->dependency_id);
				$query->range(0, 1);
				$dependency_file_data = $query->execute()->fetchObject();
				if ($dependency_file_data)
					$zip->addFile($root_path . $dependency_file_data->filepath, $EXP_PATH . $CODE_PATH . 'DEPENDENCIES/' . str_replace(' ', '_', ($dependency_file_data->filename)));
			} //$solution_dependency_files_row = $solution_dependency_files_q->fetchObject()
		} //$solution_row = $solution_q->fetchObject()
	} //$experiment_row = $experiment_q->fetchObject()
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0)
	{
		if ($user->uid)
		{
			/* download zip file */
/*			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $lab_data->lab_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_clean();
			//flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else
		{
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $lab_data->lab_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else
	{
		drupal_set_message("There are no solutions in this Lab to download", 'error');
		drupal_goto('science-and-concept-map-project/science-and-concept-map-project-run');
	}
}*/