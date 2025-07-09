<?php

function science_and_concept_map_abstract_bulk_approval_form($form, &$form_state)
{
	$options_first = _bulk_list_of_science_and_concept_map_project();
	$selected = isset($form_state['values']['science_and_concept_map_project']) ? $form_state['values']['science_and_concept_map_project'] : key($options_first);
	$form = array();
	$form['science_and_concept_map_project'] = array(
		'#type' => 'select',
		'#title' => t('Title of the science and concept map project'),
		'#options' => _bulk_list_of_science_and_concept_map_project(),
		'#default_value' => $selected,
		'#ajax' => array(
			
			'callback' => 'ajax_bulk_science_and_concept_map_abstract_details_callback'
		),
		'#suffix' => '<div id="ajax_selected_science_and_concept_map"></div><div id="ajax_selected_science_and_concept_map_pdf"></div>'
	);
	$form['science_and_concept_map_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action for science and concept map project'),
		'#options' => _bulk_list_science_and_concept_map_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_science_and_concept_map_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="science_and_concept_map_project"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Please specify the reason for Resubmission / Dis-Approval'),
		'#prefix' => '<div id= "message_submit">',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="science_and_concept_map_actions"]' => array(
						'value' => 3
					)
				),
				'or',
				array(
					':input[name="science_and_concept_map_actions"]' => array(
						'value' => 2
					)
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	return $form;
}
function ajax_bulk_science_and_concept_map_abstract_details_callback($form, $form_state)
{
	$commands = array();
	$science_and_concept_map_project_default_value = $form_state['values']['science_and_concept_map_project'];
	if ($science_and_concept_map_project_default_value != 0)
	{
		$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map', _science_and_concept_map_details($science_and_concept_map_project_default_value));
		$form['science_and_concept_map_actions']['#options'] = _bulk_list_science_and_concept_map_actions();
		$commands[] = ajax_command_replace('#ajax_selected_science_and_concept_map_action', \Drupal::service("renderer")->render($form['science_and_concept_map_actions']));
	} //$science_and_concept_map_project_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_selected_science_and_concept_map', '');
		$commands[] = ajax_command_data('#ajax_selected_science_and_concept_map', 'form_state_value_select', $form_state['values']['science_and_concept_map_project']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
/************************************************************/
function science_and_concept_map_abstract_bulk_approval_form_submit($form, &$form_state)
{
	$user = \Drupal::currentUser();
	$msg = '';
	$root_path = science_and_concept_map_document_path();
	if ($form_state['clicked_button']['#value'] == 'Submit')
	{
		if ($form_state['values']['science_and_concept_map_project'])
		// science_and_concept_map_abstract_del_lab_pdf($form_state['values']['science_and_concept_map_project']);
			if (\Drupal::currentUser()->hasPermission('soul science and concept map bulk manage abstract'))
			{
				$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
				$query->fields('soul_science_and_concept_map_proposal');
				$query->condition('id', $form_state['values']['science_and_concept_map_project']);
				$user_query = $query->execute();
				$user_info = $user_query->fetchObject();
				$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($user_info->uid);
				if ($form_state['values']['science_and_concept_map_actions'] == 1)
				{
					// approving entire project //
					$query = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts');
					$query->fields('soul_science_and_concept_map_submitted_abstracts');
					$query->condition('proposal_id', $form_state['values']['science_and_concept_map_project']);
					$abstracts_q = $query->execute();
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						\Drupal::database()->query("UPDATE {soul_science_and_concept_map_submitted_abstracts} SET abstract_approval_status = 1, is_submitted = 1, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						\Drupal::database()->query("UPDATE {soul_science_and_concept_map_submitted_abstracts_file} SET file_approval_status = 1, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					\Drupal::messenger()->addStatus(t('Approved science and concept map project.'));
					// email 
					// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_subject = t('[!site_name][Science and Concept Map Project] Your uploaded project files for science and concept map project have been approved', array(
// 						'!site_name' => variable_get('site_name', '')
// 					));

					// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_body = array(
// 						0 => t('
// 
// Dear !user_name,
// 
// Your uploaded report and project files for the science and concept map project has been approved:
// 
// Full Name : ' . $user_info->name_title . ' ' . $user_info->contributor_name . '
// Title of science and concept map project  : ' . $user_info->project_title . '
// Title of Category  : ' . $user_info->category . '
// 
// Best Wishes,
// 
// !site_name Team,
// FOSSEE,IIT Bombay', array(
// 							'!site_name' => variable_get('site_name', ''),
// 							'!user_name' => $user_data->name
// 						))
// 					);

					/** sending email when everything done **/
					$email_to = $user_data->mail;
					$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
					$bcc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
					$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('science_and_concept_map', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						$msg = \Drupal::messenger()->addError('Error sending email message.');
					} //!drupal_mail('science_and_concept_map', 'standard', $email_to, language_default(), $params, $from, TRUE)
				} //$form_state['values']['science_and_concept_map_actions'] == 1
				elseif ($form_state['values']['science_and_concept_map_actions'] == 2)
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = \Drupal::messenger()->addError("Please mention the reason for resubmission. Minimum 30 character required");
						return $msg;
					}
					//pending review entire project 
					$query = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts');
					$query->fields('soul_science_and_concept_map_submitted_abstracts');
					$query->condition('proposal_id', $form_state['values']['science_and_concept_map_project']);
					$abstracts_q = $query->execute();
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						\Drupal::database()->query("UPDATE {soul_science_and_concept_map_submitted_abstracts} SET abstract_approval_status = 0, is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						\Drupal::database()->query("UPDATE {soul_science_and_concept_map_proposal} SET is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->proposal_id
						));
						\Drupal::database()->query("UPDATE {soul_science_and_concept_map_submitted_abstracts_file} SET file_approval_status = 0, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					\Drupal::messenger()->addStatus(t('Resubmit the project files'));
					// email 
					// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_subject = t('[!site_name][Science and Concept Map Project] Your uploaded science and concept map project have been marked as pending', array(
// 						'!site_name' => variable_get('site_name', '')
// 					));

					// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_body = array(
// 						0 => t('
// 
// Dear !user_name,
// 
// Kindly resubmit the project files for the project : ' . $user_info->project_title . 'after making changes considering the following reviewer’s comments.
// 
// Comment: ' . $form_state['values']['message'] . '
// 
// Best Wishes,
// 
// !site_name Team,
// FOSSEE, IIT Bombay', array(
// 							'!site_name' => variable_get('site_name', ''),
// 							'!user_name' => $user_data->name
// 						))
// 					);

					/** sending email when everything done **/
					$email_to = $user_data->mail;
					$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
					$bcc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
					$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('science_and_concept_map', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						\Drupal::messenger()->addError('Error sending email message.');
					} //!drupal_mail('science_and_concept_map', 'standard', $email_to, language_default(), $params, $from, TRUE)
				} //$form_state['values']['science_and_concept_map_actions'] == 2
				elseif ($form_state['values']['science_and_concept_map_actions'] == 3) //disapprove and delete entire science and concept map project
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = \Drupal::messenger()->addError("Please mention the reason for disapproval. Minimum 30 character required");
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					if (!\Drupal::currentUser()->hasPermission('soul science and concept map bulk delete abstract'))
					{
						$msg = \Drupal::messenger()->addError(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Project.'));
						return $msg;
					} //!user_access('science_and_concept_map bulk delete code')
					if (science_and_concept_map_abstract_delete_project($form_state['values']['science_and_concept_map_project'])) //////
					{
						\Drupal::messenger()->addStatus(t('Dis-Approved and Deleted Entire science and concept map project.'));
						// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_subject = t('[!site_name][Science and Concept Map Project] Your uploaded science and concept map project have been marked as dis-approved', array(
// 						'!site_name' => variable_get('site_name', '')
// 					));

					// @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $email_body = array(
// 						0 => t('
// 
// Dear !user_name,
// 
// Your uploaded science and concept map project files for the science and concept map project Title : ' . $user_info->project_title . ' have been marked as dis-approved.
// 
// Reason for dis-approval: ' . $form_state['values']['message'] . '
// 
// Best Wishes,
// 
// !site_name Team,
// FOSSEE, IIT Bombay', array(
// 					'!site_name' => variable_get('site_name', ''),
// 					'!user_name' => $user_data->name
// 											))
// 					);

					$email_to = $user_data->mail;
					$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
					$bcc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
					$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('science_and_concept_map', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						\Drupal::messenger()->addError('Error sending email message.');
					}
					} //science_and_concept_map_abstract_delete_project($form_state['values']['science_and_concept_map_project'])
					else
					{
						\Drupal::messenger()->addError(t('Error Dis-Approving and Deleting Entire science and concept map project.'));
					}
					// email 
					
				} //$form_state['values']['science_and_concept_map_actions'] == 3
				/*elseif ($form_state['values']['science_and_concept_map_actions'] == 4)
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = drupal_set_message("Please mention the reason for disapproval/deletion. Minimum 30 character required", 'error');
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					$query = db_select('soul_science_and_concept_map_abstract_experiment');
					$query->fields('soul_science_and_concept_map_abstract_experiment');
					$query->condition('proposal_id', $form_state['values']['lab']);
					$query->orderBy('number', 'ASC');
					$experiment_q = $query->execute();
					$experiment_list = '';
					while ($experiment_data = $experiment_q->fetchObject())
					{
						$experiment_list .= '<p>' . $experiment_data->number . ') ' . $experiment_data->title . '<br> Description :  ' . $experiment_data->description . '<br>';
						$experiment_list .= ' ';
						$experiment_list .= '</p>';
					} //$experiment_data = $experiment_q->fetchObject()
					if (!user_access('lab migration bulk delete code'))
					{
						$msg = drupal_set_message(t('You do not have permission to Bulk Delete Entire Lab Including Proposal.'), 'error');
						return $msg;
					} //!user_access('lab migration bulk delete code')
					// check if dependency files are present 
					$dep_q = db_query("SELECT * FROM {soul_science_and_concept_map_abstract_dependency_files} WHERE proposal_id = :proposal_id", array(
						":proposal_id" => $form_state['values']['lab']
					));
					if ($dep_data = $dep_q->fetchObject())
					{
						$msg = drupal_set_message(t("Cannot delete lab since it has dependency files that can be used by others. First delete the dependency files before deleting the lab."), 'error');
						return $msg ;
					} //$dep_data = $dep_q->fetchObject()
					if (science_and_concept_map_abstract_delete_lab($form_state['values']['lab']))
					{
						drupal_set_message(t('Dis-Approved and Deleted Entire Lab solutions.'), 'status');
						$query = db_select('soul_science_and_concept_map_abstract_experiment');
						$query->fields('soul_science_and_concept_map_abstract_experiment');
						$query->condition('proposal_id', $form_state['values']['lab']);
						$experiment_q = $query->execute()->fetchObject();
						$dir_path = $root_path . $experiment_q->directory_name;
						if (is_dir($dir_path))
						{
							$res = rmdir($dir_path);
							if (!$res)
							{
								$msg = drupal_set_message(t("Cannot delete Lab directory : " . $dir_path . ". Please contact administrator."), 'error');
								return $msg;
							} //!$res
						} //is_dir($dir_path)
						else
						{
							drupal_set_message(t("Lab directory not present : " . $dir_path . ". Skipping deleting lab directory."), 'status');
						}
						$proposal_q = db_query("SELECT * FROM {soul_science_and_concept_map_abstract_proposal} WHERE id = :id", array(
							":id" => $form_state['values']['lab']
						));
						$proposal_data = $proposal_q->fetchObject();
						$proposal_id = $proposal_data->id;
						db_query("DELETE FROM {soul_science_and_concept_map_abstract_experiment} WHERE proposal_id = :proposal_id", array(
							":proposal_id" => $proposal_id
						));
						db_query("DELETE FROM {soul_science_and_concept_map_abstract_proposal} WHERE id = :id", array(
							":id" => $proposal_id
						));
						drupal_set_message(t('Deleted Lab Proposal.'), 'status');
						//email 
						$email_subject = t('[!site_name] Your uploaded Lab Migration solutions including the Lab proposal have been deleted', array(
							'!site_name' => variable_get('site_name', '')
						));
						$email_body = array(
							0 => t('

Dear !user_name,

We regret to inform you that all the uploaded Experiments of your Lab with following details have been deleted permanently.

Title of Lab :' . $user_info->lab_title . '

List of experiments : ' . $experiment_list . '

Reason for dis-approval: ' . $form_state['values']['message'] . '

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
								'!site_name' => variable_get('site_name', ''),
								'!user_name' => $user_data->name
							))
						);
						// email 
						//  $email_subject = t('Your uploaded Lab Migration solutions including the Lab proposal have been deleted');
						$email_body = array(
							0 => t('Your all the uploaded solutions including the Lab proposal have been deleted permanently.')
						);
					} //science_and_concept_map_abstract_delete_lab($form_state['values']['lab'])
					else
					{
						$msg = drupal_set_message(t('Error Dis-Approving and Deleting Entire Lab.'), 'error');
					}
				} //$form_state['values']['science_and_concept_map_actions'] == 4
				else
				{
					$msg = drupal_set_message(t('You do not have permission to bulk manage code.'), 'error');
				}*/
			} //user_access('science_and_concept_map project bulk manage code')
		return $msg;
	} //$form_state['clicked_button']['#value'] == 'Submit'
}
/**********************************************************/
function _bulk_list_of_science_and_concept_map_project()
{
	$project_titles = array(
		'0' => 'Please select...'
	);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('is_submitted', 1);
	$query->condition('approval_status', 1);
	$query->orderBy('project_title', 'ASC');
	$project_titles_q = $query->execute();
	while ($project_titles_data = $project_titles_q->fetchObject())
	{
		$project_titles[$project_titles_data->id] = $project_titles_data->project_title . ' (Proposed by ' . $project_titles_data->contributor_name . ')';
	} //$project_titles_data = $project_titles_q->fetchObject()
	return $project_titles;
}
function _bulk_list_science_and_concept_map_actions()
{
	$science_and_concept_map_actions = array(
		0 => 'Please select...'
	);
	$science_and_concept_map_actions[1] = 'Approve Entire science and concept map Project';
	$science_and_concept_map_actions[2] = 'Resubmit Project files';
	$science_and_concept_map_actions[3] = 'Dis-Approve Entire science and concept map Project (This will delete science and concept map Project)';
	//$science_and_concept_map_actions[4] = 'Delete Entire science and concept map Project Including Proposal';
	return $science_and_concept_map_actions;
}
function _science_and_concept_map_details($science_and_concept_map_proposal_id)
{
	$return_html = "";
	$query_pro = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query_pro->fields('soul_science_and_concept_map_proposal');
	$query_pro->condition('id', $science_and_concept_map_proposal_id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
	//var_dump($abstracts_pro);die;
	$query_pdf = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts_file');
	$query_pdf->fields('soul_science_and_concept_map_submitted_abstracts_file');
	$query_pdf->condition('proposal_id', $science_and_concept_map_proposal_id);
	$query_pdf->condition('filetype', 'A');
	$abstracts_pdf = $query_pdf->execute()->fetchObject();
	if ($abstracts_pdf == TRUE)
	{
		if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "")
		{
			$abstract_filename = $abstracts_pdf->filename;
		} //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
		else
		{
			$abstract_filename = "File not uploaded";
		}
	} //$abstracts_pdf == TRUE
	else
	{
		$abstract_filename = "File not uploaded";
	}
	$query_process = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts_file');
	$query_process->fields('soul_science_and_concept_map_submitted_abstracts_file');
	$query_process->condition('proposal_id', $science_and_concept_map_proposal_id);
	$query_process->condition('filetype', 'S');
	$abstracts_query_process = $query_process->execute()->fetchObject();
	$query = \Drupal::database()->select('soul_science_and_concept_map_submitted_abstracts');
	$query->fields('soul_science_and_concept_map_submitted_abstracts');
	$query->condition('proposal_id', $science_and_concept_map_proposal_id);
	$abstracts_q = $query->execute()->fetchObject();
	if ($abstracts_q)
	{
		if ($abstracts_q->is_submitted == 0)
		{
			//drupal_set_message(t('Abstract is not submmited yet.'), 'error', $repeat = FALSE);
			//return;
		} //$abstracts_q->is_submitted == 0
	} //$abstracts_q
	//var_dump($abstracts_query_process);die;
	if ($abstracts_query_process == TRUE)
	{
		if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "")
		{
			$abstracts_query_process_filename = $abstracts_query_process->filename;
		} //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
		else
		{
			$abstracts_query_process_filename = "File not uploaded";
		}
	} //$abstracts_query_process == TRUE
	else
	{
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $url = l('Upload abstract', 'science-and-concept-map-project/abstract-code/upload');

		$abstracts_query_process_filename = "File not uploaded";
	}
	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $download_science_and_concept_map = l('Download science and concept map project','science-and-concept-map-project/full-download/project/'.$science_and_concept_map_proposal_id);

	$return_html .= '<strong>Proposer Name:</strong><br />' . $abstracts_pro->name_title . ' ' . $abstracts_pro->contributor_name . '<br /><br />';
	$return_html .= '<strong>Title of the science and concept map Project:</strong><br />' . $abstracts_pro->project_title . '<br /><br />';
	$return_html .= '<strong>Title of the Category:</strong><br />' . $abstracts_pro->category . '<br /><br />';
	$return_html .= '<strong>Uploaded a Report (brief outline) of the project:</strong><br />' . $abstract_filename . '<br /><br />';
	$return_html .= '<strong>Upload the soul science and concept map project code files for the proposal:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
	$return_html .= $download_science_and_concept_map;
	return $return_html;
}