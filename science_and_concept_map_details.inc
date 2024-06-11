<?php
// soul science and concept map display completed proposals
function science_and_concept_map_completed_proposals_all()
{
	$output = "";
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('actual_completion_date', 'DESC');
	//$query->condition('is_completed', 1);
	$result = $query->execute();
	
	//var_dump($soul_project_abstract);die;
	if ($result->rowCount() == 0)
	{
		$output .= "We welcome your contributions." ."<hr>";

	} //$result->rowCount() == 0
	else
	{
		$output .= "Work has been completed for the following Science and Concept Map." . "<hr>";
		$preference_rows = array();
		$i = $result->rowCount();
		while ($row = $result->fetchObject())
		{
			$proposal_id = $row->id;
			$query1 = db_select('soul_science_and_concept_map_submitted_abstracts_file');
			$query1->fields('soul_science_and_concept_map_submitted_abstracts_file');
			$query1->condition('file_approval_status', 1);
			$query1->condition('proposal_id', $proposal_id);
			$soul_project_files = $query1->execute();
			$soul_project_abstract = $soul_project_files->fetchObject();
			$completion_date = date("Y", $row->actual_completion_date);
			$preference_rows[] = array(
				$i,
				l($row->project_title, "science-and-concept-map-project/soul-science-and-concept-map-run/" . $row->id),
				$row->contributor_name,
				$row->university,
				$completion_date
			);
			$i--;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No.',
			'Science and Concept Map Project',
			'Contributor Name',
			'University / Institute',
			'Year of Completion'
		);
		$output .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $output;
}
// soul science and concept map display in progress proposals
function science_and_concept_map_progress_all()
{
	$page_content = "";
	$query = db_select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 1);
	$query->condition('is_completed', 0);
	$query->orderBy('approval_date', 'DESC');
	$result = $query->execute();
	if ($result->rowCount() == 0)
	{
		$page_content .= "We welcome your contributions.<hr>";
	} //$result->rowCount() == 0
	else
	{
		$i = $result->rowCount();
		$page_content .= "Work is in progress for the following Science and Concept Map<hr>";
		$preference_rows = array();
		// $i = 1;
		while ($row = $result->fetchObject())
		{
			$approval_date = date("Y", $row->approval_date);
			$preference_rows[] = array(
				$i,
				$row->project_title,
				$row->contributor_name,
				$row->university,
				$approval_date
			);
			$i--;
		} //$row = $result->fetchObject()
		$preference_header = array(
			'No',
			'Science and Concept Map Project',
			'Contributor Name',
			'Institute',
			'Year'
		);
		$page_content .= theme('table', array(
			'header' => $preference_header,
			'rows' => $preference_rows
		));
	}
	return $page_content;
}
