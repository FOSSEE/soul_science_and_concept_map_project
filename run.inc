<?php
function science_and_concept_map_run_form($form, &$form_state)
{
	$options_first = _list_of_science_and_concept_map();
	$url_science_and_concept_map_id = (int) arg(2);
	$science_and_concept_map_data = _science_and_concept_map_information($url_science_and_concept_map_id);
	if ($science_and_concept_map_data == 'Not found') {
		$url_science_and_concept_map_id = '';
	} //$science_and_concept_map_data == 'Not found'
	if (!$url_science_and_concept_map_id) {
		$selected = isset($form_state['values']['science_and_concept_map']) ? $form_state['values']['science_and_concept_map'] : key($options_first);
	} //!$url_science_and_concept_map_id
	elseif ($url_science_and_concept_map_id == '') {
		$selected = 0;
	} //$url_science_and_concept_map_id == ''
	else {
		$selected = $url_science_and_concept_map_id;
	}
	$form = array();
	$form['science_and_concept_map'] = array(
		'#type' => 'select',
		'#title' => t('Title of the science and concept map'),
		'#options' => _list_of_science_and_concept_map(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'science_and_concept_map_project_details_callback'
		)
	);
	if (!$url_science_and_concept_map_id) {
		$form['science_and_concept_map_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_science_and_concept_map_details"></div>'
		);
		$form['selected_science_and_concept_map'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_selected_science_and_concept_map"></div>'
		);
	} //!$url_science_and_concept_map_id
	else {
		$science_and_concept_map_default_value = $url_science_and_concept_map_id;
		$form['science_and_concept_map_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_science_and_concept_map_details">' . _science_and_concept_map_details($science_and_concept_map_default_value) . '</div>'
		);
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['selected_science_and_concept_map'] = array(
// 			'#type' => 'item',
// 			'#markup' => '<div id="ajax_selected_science_and_concept_map">' . l('Download Abstract', "science-and-concept-map-project/download/abstract-file/" . $science_and_concept_map_default_value) . '<br>' . l('Download science and concept map', 'science-and-concept-map-project/full-download/project/' . $science_and_concept_map_default_value) . '</div>'
// 		);

	}
	return $form;
}
function science_and_concept_map_project_details_callback($form, $form_state)
{
	$commands = array();
	$science_and_concept_map_default_value = $form_state['values']['science_and_concept_map'];
	if ($science_and_concept_map_default_value != 0) {
		$form['science_and_concept_map_details']['#markup'] = _science_and_concept_map_details($science_and_concept_map_default_value);
		$science_and_concept_map_details = _science_and_concept_map_information($science_and_concept_map_default_value);
		$provider = \Drupal::entityTypeManager()->getStorage('user')->load($science_and_concept_map_details->uid);
		if ($science_and_concept_map_details->uid > 0) {
			// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $commands[] = ajax_command_html('#ajax_selected_science_and_concept_map',l('Download Abstract', "science-and-concept-map-project/download/abstract-file/" . $science_and_concept_map_default_value) . '<br>' . l('Download science and concept map', 'science-and-concept-map-project/full-download/project/' . $science_and_concept_map_default_value));

		} //$science_and_concept_map_details->uid > 0
		else {
			$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map', '');
			$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map_soul', '');
		}
		$commands[] = ajax_command_html('#ajax_science_and_concept_map_details', _science_and_concept_map_details($science_and_concept_map_default_value));
	} //$science_and_concept_map_default_value != 0
	else {
		// $form['lab_experiment_list']['#options'] = _ajax_get_experiment_list();
		//  $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_science_and_concept_map_details', '');
		$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map', '');
		$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map_soul', '');
		$commands[] = ajax_command_data('#ajax_selected_science_and_concept_map', 'form_state_value_select', $form_state['values']['science_and_concept_map']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header) {
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row) {
		$tbody .= "<tr>";
		foreach ($row as $data) {
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}
/*****************************************************/
function _list_of_science_and_concept_map()
{
	$science_and_concept_map_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {science_and_concept_map_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('project_title', 'ASC');
	$science_and_concept_map_titles_q = $query->execute();
	while ($science_and_concept_map_titles_data = $science_and_concept_map_titles_q->fetchObject()) {
		$science_and_concept_map_titles[$science_and_concept_map_titles_data->id] = $science_and_concept_map_titles_data->project_title . ' (Proposed by ' . $science_and_concept_map_titles_data->name_title . ' ' . $science_and_concept_map_titles_data->contributor_name . ')';
	} //$science_and_concept_map_titles_data = $science_and_concept_map_titles_q->fetchObject()
	return $science_and_concept_map_titles;
}
function _science_and_concept_map_information($proposal_id)
{
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$query->condition('approval_status', 3);
	$science_and_concept_map_q = $query->execute();
	$science_and_concept_map_data = $science_and_concept_map_q->fetchObject();
	if ($science_and_concept_map_data) {
		return $science_and_concept_map_data;
	} //$science_and_concept_map_data
	else {
		return 'Not found';
	}
}
function _science_and_concept_map_details($science_and_concept_map_default_value)
{
	$science_and_concept_map_details = _science_and_concept_map_information($science_and_concept_map_default_value);
	if ($science_and_concept_map_default_value != 0) {
		if($science_and_concept_map_details->reference != NULL){
		$url = '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i';
	        $reference = preg_replace($url, '<a href="$0" target="_blank" title="$0">$0</a>', $science_and_concept_map_details->reference);
		}else{
		$reference = 'Not provided';
		}
		$form['science_and_concept_map_details']['#markup'] = '<span style="color: rgb(128, 0, 0);"><strong>About the science and concept map</strong></span></td><td style="width: 35%;"><br />' . '<ul>' . '<li><strong>Proposer Name:</strong> ' . $science_and_concept_map_details->name_title . ' ' . $science_and_concept_map_details->contributor_name . '</li>' . '<li><strong>Title of the science and concept map:</strong> ' . '<span style="color: #5D125D;">' .$science_and_concept_map_details->project_title. '</li>' . '<li><strong>University:</strong> ' . $science_and_concept_map_details->university . '</li>' . '<li><strong>Category:</strong> ' . $science_and_concept_map_details->category . '</li>' . '<li>'.'<strong>Reference:</strong> ' . $reference .'</li>'.'</ul>';
		$details = $form['science_and_concept_map_details']['#markup'];
		return $details;
	} //$science_and_concept_map_default_value != 0
}