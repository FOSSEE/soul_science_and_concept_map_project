<?php
// $Id$
function science_and_concept_map_proposal_pending()
{
	/* get pending proposals to be approved */
	$pending_rows = array();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('approval_status', 0);
	$query->orderBy('id', 'DESC');
	$pending_q = $query->execute();
	while ($pending_data = $pending_q->fetchObject())
	{
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $pending_rows[$pending_data->id] = array(
// 			date('d-m-Y', $pending_data->creation_date),
// 			l($pending_data->name_title . ' ' . $pending_data->contributor_name, 'user/' . $pending_data->uid),
// 			$pending_data->project_title,
// 			l('Approve', 'science-and-concept-map-project/manage-proposal/approve/' . $pending_data->id) . ' | ' . l('Edit', 'science-and-concept-map-project/manage-proposal/edit/' . $pending_data->id)
// 		);

	} //$pending_data = $pending_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$pending_rows)
	{
		\Drupal::messenger()->addStatus(t('There are no pending proposals.'));
		return '';
	} //!$pending_rows
	$pending_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the science and concept map Project',
		'Action'
	);
	//$output = theme_table($pending_header, $pending_rows);
	// @FIXME
// theme() has been renamed to _theme() and should NEVER be called directly.
// Calling _theme() directly can alter the expected output and potentially
// introduce security issues (see https://www.drupal.org/node/2195739). You
// should use renderable arrays instead.
// 
// 
// @see https://www.drupal.org/node/2195739
// $output = theme('table', array(
// 		'header' => $pending_header,
// 		'rows' => $pending_rows
// 	));

	return $output;
}
function science_and_concept_map_proposal_all()
{
	/* get pending proposals to be approved */
	$proposal_rows = array();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->orderBy('id', 'DESC');
	$proposal_q = $query->execute();
	while ($proposal_data = $proposal_q->fetchObject())
	{
		$approval_status = '';
		switch ($proposal_data->approval_status)
		{
			case 0:
				$approval_status = 'Pending';
				break;
			case 1:
				$approval_status = 'Approved';
				break;
			case 2:
				$approval_status = 'Dis-approved';
				break;
			case 3:
				$approval_status = 'Completed';
				break;
			default:
				$approval_status = 'Unknown';
				break;
		} //$proposal_data->approval_status
		if ($proposal_data->actual_completion_date == 0)
		{
			$actual_completion_date = "Not Completed";
		} //$proposal_data->actual_completion_date == 0
		else
		{
			$actual_completion_date = date('d-m-Y', $proposal_data->actual_completion_date);
		}
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $proposal_rows[] = array(
// 			date('d-m-Y', $proposal_data->creation_date),
// 			l($proposal_data->contributor_name, 'user/' . $proposal_data->uid),
// 			$proposal_data->project_title,
// 			$actual_completion_date,
// 			$approval_status,
// 			l('Status', 'science-and-concept-map-project/manage-proposal/status/' . $proposal_data->id) . ' | ' . l('Edit', 'science-and-concept-map-project/manage-proposal/edit/' . $proposal_data->id)
// 		);

	} //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$proposal_rows)
	{
		\Drupal::messenger()->addStatus(t('There are no proposals.'));
		return '';
	} //!$proposal_rows
	$proposal_header = array(
		'Date of Submission',
		'Student Name',
		'Title of the science-and-concept-map project',
		'Date of Completion',
		'Status',
		'Action'
	);
	// @FIXME
// theme() has been renamed to _theme() and should NEVER be called directly.
// Calling _theme() directly can alter the expected output and potentially
// introduce security issues (see https://www.drupal.org/node/2195739). You
// should use renderable arrays instead.
// 
// 
// @see https://www.drupal.org/node/2195739
// $output = theme('table', array(
// 		'header' => $proposal_header,
// 		'rows' => $proposal_rows
// 	));

	return $output;
}
/******************************************************************************/
/************************** PROPOSAL APPROVAL FORM ****************************/
/******************************************************************************/
function science_and_concept_map_proposal_approval_form($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = \Drupal::database()->select('soul_science_and_concept_map_textbook_details');
	$query ->fields('soul_science_and_concept_map_textbook_details');
	$query-> condition('proposal_id', $proposal_id);
	$book_q = $query->execute();
	$book_data = $book_q->fetchObject();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
	{
		$project_guide_name = "Not Entered";
	} //$proposal_data->project_guide_name == NULL
	else
	{
		$project_guide_name = $proposal_data->project_guide_name;
	}
	if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
	{
		$project_guide_email_id = "Not Entered";
	} //$proposal_data->project_guide_email_id == NULL
	else
	{
		$project_guide_email_id = $proposal_data->project_guide_email_id;
	}
	if ($proposal_data->project_guide_department == "NULL" || $proposal_data->project_guide_department == "")
	{
		$project_guide_department = "Not Entered";
	} //$proposal_data->project_guide_email_id == NULL
	else
	{
		$project_guide_department = $proposal_data->project_guide_department;
	}
	if ($proposal_data->project_guide_university == "NULL" || $proposal_data->project_guide_university == "")
	{
		$project_guide_university = "Not Entered";
	} //$proposal_data->project_guide_email_id == NULL
	else
	{
		$project_guide_university = $proposal_data->project_guide_university;
	}
	$query = \Drupal::database()->select('soul_science_and_concept_map_software_version');
	$query->fields('soul_science_and_concept_map_software_version');
	$query->condition('id', $proposal_data->software_version);
	$software_version_data = $query->execute()->fetchObject();
	if(!$software_version_data){
		$software_versions = 'NA';
	}
	else{
		$software_versions = $software_version_data->software_versions;
	}
	if ($proposal_data->second_software == "NULL" || $proposal_data->second_software == "")
	{
		$second_software = "Not Entered";

	} //$proposal_data->city == NULL
	else
	{
		$second_software = $proposal_data->second_software;
	}
	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['contributor_name'] = array(
// 		'#type' => 'item',
// 		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
// 		'#title' => t('Contributor name')
// 	);

	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid)->mail,
		// '#title' => t('Email')
	);
	$form['contributor_contact_no'] = array(
		'#title' => t('Contact No.'),
		'#type' => 'item',
		'#markup' => $proposal_data->contact_no,
	);
	/*$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'd-M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#disabled' => TRUE
	);*/
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['department'] = array(
		'#type' => 'item',
		'#title' => t('Department/Branch'),
		'#markup' => $proposal_data->department,
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->country,
		'#title' => t('Country')
	);
	if($proposal_data->country == 'others'){
	$form['other_country'] = array(
		'#type' => 'item',
		'#title' => t('Other than India'),
		'#markup' => $proposal_data->other_country,
	);
	}
	$form['all_state'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->state,
		'#title' => t('State')
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->city,
		'#title' => t('City')
	);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);

	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Name of the faculty member of your Institution, if any, who helped you with this project '),
		'#markup' => $project_guide_name
	);
	$form['project_guide_department'] = array(
		'#type' => 'item',
		'#title' => t('Department of the faculty member of your Institution, if any, who helped you with this project '),
		'#markup' => $project_guide_department
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Email id of the faculty member of your Institution, if any, who helped you with this project'),
		'#markup' =>$project_guide_email_id
	);
	$form['project_guide_university'] = array(
		'#type' => 'item',
		'#title' => t('University name of the faculty member of your Institution, if any, who helped you with this project'),
		'#markup' =>$project_guide_university
	);
	$form['options'] = array(
		'#type' => 'item',
		'#title' => t('How did you come to know about the Science and Concept Map project'),
		'#markup' =>$proposal_data->options
	);
	$form['category'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->category,
		'#title' => t('Category')
	);
	$form['sub_category'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->sub_category,
		'#title' => t('Sub Category')
	);
	$form['software_version'] = array(
		'#type' => 'item',
		'#markup' => $software_versions,
		'#title' => t('Software Version')
	);
	$form['software_version_no'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->software_version_no,
		'#title' => t('Software Version No')
	);
	$form['other_software_version_no'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->other_software_version_no,
		'#title' => t('Other Software Version No')
	);
	$form['second_software'] = array(
		'#type' => 'item',
		'#markup' => $second_software,
		'#title' => t('Second Software Version')
	);
	if($proposal_data->is_ncert_book == 'Yes'){
		$form['book_name'] = array(
			'#type' => 'item',
			'#markup' => $book_data->book,
			'#title' => t('Book name')
		);
		$form['author_name'] = array(
			'#type' => 'item',
			'#markup' => $book_data->author,
			'#title' => t('Author name')
		);
		$form['isbn_no'] = array(
			'#type' => 'item',
			'#markup' => $book_data->isbn,
			'#title' => t('ISBN no.')
		);
		$form['publisher'] = array(
			'#type' => 'item',
			'#markup' => $book_data->publisher,
			'#title' => t('Publisher and place')
		);
		$form['edition'] = array(
			'#type' => 'item',
			'#markup' => $book_data->edition,
			'#title' => t('Edition of a book')
		);
		$form['book_year'] = array(
			'#type' => 'item',
			'#markup' => $book_data->book_year,
			'#title' => t('Year of publication')
		);
	}
	$form['year_of_study'] = array(
		'#type' => 'item',
		'#markup' => $book_data->year_of_study,
		'#title' => t('The project is suitable for class (school education)/year of study(college education)')
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the Science and Concept Map Project')
	);
	$form['description'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->description,
		'#title' => t('Description of the Science and Concept Map Project')
	);
	$form['reference'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->reference,
		'#title' => t('Reference')
	);
	if (($proposal_data->abstractfilepath != "") && ($proposal_data->abstractfilepath != 'NULL'))
	{
		$str = substr($proposal_data->abstractfilepath,strrpos($proposal_data->abstractfilepath, '/'));
		$resource_file =ltrim($str, '/');
		
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['abstractfilepath'] = array(
// 			'#type' => 'item',
// 			'#title' => t('Abstract file '),
// 			'#markup' => l($resource_file, 'science-and-concept-map-project/download/abstract-file/' . $proposal_id) . ""
// 		);

	} //$proposal_data->user_defined_compound_filepath != ""
	else
	{
		$form['abstractfilepath'] = array(
			'#type' => 'item',
			'#title' => t('Abstract file '),
			'#markup' => "Not uploaded<br><br>"
		);
	}
	$form['approval'] = array(
		'#type' => 'radios',
		'#title' => t('Soul Science and Concept Map Proposal'),
		'#options' => array(
			'1' => 'Approve',
			'2' => 'Disapprove'
		),
		'#required' => TRUE
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Reason for disapproval'),
		'#attributes' => array(
			'placeholder' => t('Enter reason for disapproval in minimum 30 characters '),
			'cols' => 50,
			'rows' => 4
		),
		'#states' => array(
			'visible' => array(
				':input[name="approval"]' => array(
					'value' => '2'
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['cancel'] = array(
// 		'#type' => 'item',
// 		'#markup' => l(t('Cancel'), 'science-and-concept-map-project/manage-proposal')
// 	);

	return $form;
}
function science_and_concept_map_proposal_approval_form_validate($form, &$form_state)
{
	if ($form_state['values']['approval'] == 2)
	{
		if ($form_state['values']['message'] == '')
		{
			form_set_error('message', t('Reason for disapproval could not be empty'));
		} //$form_state['values']['message'] == ''
	} //$form_state['values']['approval'] == 2
}
function science_and_concept_map_proposal_approval_form_submit($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	if ($form_state['values']['approval'] == 1)
	{
		$query = "UPDATE {soul_science_and_concept_map_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":proposal_id" => $proposal_id
		);
		\Drupal::database()->query($query, $args);
		/* sending email */
		$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
		$bcc = $user->mail . ', ' . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
		$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
		$params['science_and_concept_map_proposal_approved']['proposal_id'] = $proposal_id;
		$params['science_and_concept_map_proposal_approved']['user_id'] = $proposal_data->uid;
		$params['science_and_concept_map_proposal_approved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_approved', $email_to, language_default(), $params, $from, TRUE))
			\Drupal::messenger()->addError('Error sending email message.');
		\Drupal::messenger()->addStatus('soul science-and-concept-map proposal No. ' . $proposal_id . ' approved. User has been notified of the approval.');
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 1
	else if ($form_state['values']['approval'] == 2)
	{
		$query = "UPDATE {soul_science_and_concept_map_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, dissapproval_reason = :dissapproval_reason WHERE id = :proposal_id";
		$args = array(
			":uid" => $user->uid,
			":date" => time(),
			":dissapproval_reason" => $form_state['values']['message'],
			":proposal_id" => $proposal_id
		);
		$result = \Drupal::database()->query($query, $args);
		/* sending email */
		$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
		$bcc = $user->mail . ', ' . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
		$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
		$params['science_and_concept_map_proposal_disapproved']['proposal_id'] = $proposal_id;
		$params['science_and_concept_map_proposal_disapproved']['user_id'] = $proposal_data->uid;
		$params['science_and_concept_map_proposal_disapproved']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_disapproved', $email_to, language_default(), $params, $from, TRUE))
			\Drupal::messenger()->addError('Error sending email message.');
		\Drupal::messenger()->addError('soul science and concept map proposal No. ' . $proposal_id . ' dis-approved. User has been notified of the dis-approval.');
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	} //$form_state['values']['approval'] == 2
}
/******************************************************************************/
/*************************** PROPOSAL STATUS FORM *****************************/
/******************************************************************************/
function science_and_concept_map_proposal_status_form($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = \Drupal::database()->select('soul_science_and_concept_map_textbook_details');
	$query ->fields('soul_science_and_concept_map_textbook_details');
	$query-> condition('proposal_id', $proposal_id);
	$book_q = $query->execute();
	$book_data = $book_q->fetchObject();
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	if ($proposal_data->country == "NULL" || $proposal_data->country == "")
	{
		$country = "Not Entered";
	} //$proposal_data->country == NULL
	else
	{
		$country = $proposal_data->country;
	}
	if ($proposal_data->state == "NULL" || $proposal_data->state == "")
	{
		$state = "Not Entered";
	} //$proposal_data->state == NULL
	else
	{
		$state = $proposal_data->state;
	}
	if ($proposal_data->city == "NULL" || $proposal_data->city == "")
	{
		$city = "Not Entered";
	} //$proposal_data->city == NULL
	else
	{
		$city = $proposal_data->city;
	}
	if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
	{
		$project_guide_name = "Not Entered";
	} //$proposal_data->project_guide_name == NULL
	else
	{
		$project_guide_name = $proposal_data->project_guide_name;
	}
	if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
	{
		$project_guide_email_id = "Not Entered";
	} //$proposal_data->project_guide_email_id == NULL
	else
	{
		$project_guide_email_id = $proposal_data->project_guide_email_id;
	}
	if ($proposal_data->project_guide_university == "NULL" || $proposal_data->project_guide_university == "")
	{
		$project_guide_university = "Not Entered";
	} //$proposal_data->project_guide_university == NULL
	else
	{
		$project_guide_university = $proposal_data->project_guide_university;
	}
	if ($proposal_data->second_software == "NULL" || $proposal_data->second_software == "")
	{
		$second_software = "Not Entered";
	} //$proposal_data->city == NULL
	else
	{
		$second_software = $proposal_data->second_software;
	}
	$query = \Drupal::database()->select('soul_science_and_concept_map_software_version');
	$query->fields('soul_science_and_concept_map_software_version');
	$query->condition('id', $proposal_data->software_version);
	$software_version_data = $query->execute()->fetchObject();
	if(!$software_version_data){
		$software_versions = 'NA';
	}
	else{
		$software_versions = $software_version_data->software_versions;
	}

	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['contributor_name'] = array(
// 		'#type' => 'item',
// 		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
// 		'#title' => t('Student name')
// 	);

	$form['student_email_id'] = array(
		'#title' => t('Student Email'),
		'#type' => 'item',
		'#markup' => \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid)->mail,
		'#title' => t('Email')
	);
	/*$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#disabled' => TRUE
	);*/
	$form['university'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->university,
		'#title' => t('University/Institute')
	);
	$form['department'] = array(
		'#type' => 'item',
		'#title' => t('Department/Branch'),
		'#markup' => $proposal_data->department,
	);
	$form['country'] = array(
		'#type' => 'item',
		'#markup' => $country,
		'#title' => t('Country')
	);
	$form['state'] = array(
		'#type' => 'item',
		'#markup' => $state,
		'#title' => t('State')
	);
	$form['category'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->category,
		'#title' => t('Select Category'),
	);
	$form['city'] = array(
		'#type' => 'item',
		'#markup' => $city,
		'#title' => t('City')
		//ashvdjas
		);
	$form['pincode'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->pincode,
		'#title' => t('Pincode/Postal code')
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $project_guide_name
	);
	$form['project_guide_department'] = array(
		'#type' => 'item',
		'#title' => t('Department of the faculty member of your Institution, if any, who helped you with this project '),
		'#markup' => $proposal_data->project_guide_department
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $project_guide_email_id
	);
	$form['project_guide_university'] = array(
		'#type' => 'item',
		'#title' => t('University of the faculty member of your Institution, if any, who helped you with this project '),
		'#markup' => $proposal_data->project_guide_university
	);
	$form['category'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->category,
		'#title' => t('Category')
	);
	$form['sub_category'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->sub_category,
		'#title' => t('Sub Category')
	);
	$form['software_version'] = array(
		'#type' => 'item',
		'#markup' => $software_versions,
		'#title' => t('Software Version')
	);
	$form['software_version_no'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->software_version_no,
		'#title' => t('Software Version No')
	);
	$form['other_software_version_no'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->other_software_version_no,
		'#title' => t('Other Software Version No')
	);
	$form['second_software'] = array(
		'#type' => 'item',
		'#markup' => $second_software,
		'#title' => t('Second Software Version')
	);
	$form['project_guide_name'] = array(
		'#type' => 'item',
		'#title' => t('Project guide'),
		'#markup' => $project_guide_name
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Project guide email'),
		'#markup' => $project_guide_email_id
	);
	if($proposal_data->is_ncert_book == 'Yes'){
		$form['book_name'] = array(
			'#type' => 'item',
			'#markup' => $book_data->book,
			'#title' => t('Book name')
		);
		$form['author_name'] = array(
			'#type' => 'item',
			'#markup' => $book_data->author,
			'#title' => t('Author name')
		);
		$form['isbn_no'] = array(
			'#type' => 'item',
			'#markup' => $book_data->isbn,
			'#title' => t('ISBN no.')
		);
		$form['publisher'] = array(
			'#type' => 'item',
			'#markup' => $book_data->publisher,
			'#title' => t('Publisher and place')
		);
		$form['edition'] = array(
			'#type' => 'item',
			'#markup' => $book_data->edition,
			'#title' => t('Edition of a book')
		);
		$form['book_year'] = array(
			'#type' => 'item',
			'#markup' => $book_data->book_year,
			'#title' => t('Year of publication')
		);
	}
	$form['year_of_study'] = array(
		'#type' => 'item',
		'#markup' => $book_data->year_of_study,
		'#title' => t('The project is suitable for class (school education)/year of study(college education)')
	);
	$form['project_title'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->project_title,
		'#title' => t('Title of the science and concept map Project')
	);
	$form['description'] = array(
		'#type' => 'item',
		'#markup' => $proposal_data->description,
		'#title' => t('Description of the science and concept map Project')
	);
	/************************** reference link filter *******************/
	$url = '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i';
	$reference = preg_replace($url, '<a href="$0" target="_blank" title="$0">$0</a>', $proposal_data->reference);
	/******************************/
	$form['reference'] = array(
		'#type' => 'item',
		'#markup' => $reference,
		'#title' => t('References')
	);
	$proposal_status = '';
	switch ($proposal_data->approval_status)
	{
		case 0:
			$proposal_status = t('Pending');
			break;
		case 1:
			$proposal_status = t('Approved');
			break;
		case 2:
			$proposal_status = t('Dis-approved');
			break;
		case 3:
			$proposal_status = t('Completed');
			break;
		default:
			$proposal_status = t('Unkown');
			break;
	} 
	$form['proposal_status'] = array(
		'#type' => 'item',
		'#markup' => $proposal_status,
		'#title' => t('Proposal Status')
	);
	if ($proposal_data->approval_status == 0)
	{
		// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['approve'] = array(
// 			'#type' => 'item',
// 			'#markup' => l('Click here', 'science-and-concept-map-project/manage-proposal/approve/' . $proposal_id),
// 			'#title' => t('Approve')
// 		);

	} //$proposal_data->approval_status == 0
	if ($proposal_data->approval_status == 1)
	{
		$form['completed'] = array(
			'#type' => 'checkbox',
			'#title' => t('Completed'),
			'#description' => t('Check if user has provided all the required files and pdfs.')
		);
	} //$proposal_data->approval_status == 1
	if ($proposal_data->approval_status == 2)
	{
		$form['message'] = array(
			'#type' => 'item',
			'#markup' => $proposal_data->message,
			'#title' => t('Reason for disapproval')
		);
	} //$proposal_data->approval_status == 2
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['cancel'] = array(
// 		'#type' => 'markup',
// 		'#markup' => l(t('Cancel'), 'science-and-concept-map-project/manage-proposal/all')
// 	);

	return $form;
}
function science_and_concept_map_proposal_status_form_submit($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	//$proposal_q = db_query("SELECT * FROM {soul_science_and_concept_map_proposal} WHERE id = %d", $proposal_id);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	/* set the book status to completed */
	if ($form_state['values']['completed'] == 1)
	{
		$up_query = "UPDATE soul_science_and_concept_map_proposal SET approval_status = :approval_status , actual_completion_date = :expected_completion_date WHERE id = :proposal_id";
		$args = array(
			":approval_status" => '3',
			":proposal_id" => $proposal_id,
			":expected_completion_date" => time()
		);
		$result = \Drupal::database()->query($up_query, $args);
		CreateReadmeFileSoulScienceAndConceptMapProject($proposal_id);
		if (!$result)
		{
			\Drupal::messenger()->addError('Error in update status');
			return;
		} //!$result
		/* sending email */
		$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
		$bcc = $user->mail . ', ' . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
		$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
		$params['science_and_concept_map_proposal_completed']['proposal_id'] = $proposal_id;
		$params['science_and_concept_map_proposal_completed']['user_id'] = $proposal_data->uid;
		$params['science_and_concept_map_proposal_completed']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_completed', $email_to, language_default(), $params, $from, TRUE))
			\Drupal::messenger()->addError('Error sending email message.');
		\Drupal::messenger()->addStatus('Congratulations! soul science and concept map proposal has been marked as completed. User has been notified of the completion.');
	}
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	
}
/******************************************************************************/
/**************************** PROPOSAL EDIT FORM ******************************/
/******************************************************************************/
function science_and_concept_map_proposal_edit_form($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = \Drupal::database()->select('soul_science_and_concept_map_textbook_details');
	$query ->fields('soul_science_and_concept_map_textbook_details');
	$query-> condition('proposal_id', $proposal_id);
	$book_q = $query->execute();
	$book_data = $book_q->fetchObject();
	//$proposal_q = db_query("SELECT * FROM {soul_science_and_concept_map_proposal} WHERE id = %d", $proposal_id);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	$query = \Drupal::database()->select('soul_science_and_concept_map_software_version');
	$query->fields('soul_science_and_concept_map_software_version');
	$query->condition('id', $proposal_data->software_version);
	$software_version_data = $query->execute()->fetchObject();
	if(!$software_version_data){
		$software_versions = 'NA';
	}
	else{
		$software_versions = $software_version_data->software_versions;
	}
	if ($proposal_data->second_software == "NULL" || $proposal_data->second_software == "")
	{
		$second_software = "Not Entered";
	} //$proposal_data->city == NULL
	else
	{
		$second_software = $proposal_data->second_software;
	}
	$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof',
			'Mr' => 'Mr',
			'Mrs' => 'Mrs',
			'Ms' => 'Ms'
		),
		'#required' => TRUE,
		'#default_value' => $proposal_data->name_title
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Proposer'),
		'#size' => 100,
		'#maxlength' => 500,
		'#required' => TRUE,
		'#default_value' => $proposal_data->contributor_name
	);
	$form['student_email_id'] = array(
		'#type' => 'item',
		'#title' => t('Email'),
		'#markup' => $user_data->mail
	);
	/*$form['month_year_of_degree'] = array(
		'#type' => 'date_popup',
		'#title' => t('Month and year of award of degree'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => $proposal_data->month_year_of_degree,
		'#date_format' => 'M-Y',
		'#date_increment' => 0,
		'#date_year_range' => '1960:+0',
		'#datepicker_options' => array(
			'maxDate' => 0
		),
		'#disabled' => TRUE
	);*/
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University/Institute'),
		'#size' => 50,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#default_value' => $proposal_data->university
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#default_value' => $proposal_data->country,
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	/*$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#default_value' => $proposal_data->country,
		'#attributes' => array(
			'placeholder' => t('Enter your country name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);*/
	/*$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#default_value' => $proposal_data->state,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);*/
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
		'#default_value' => $proposal_data->city,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _df_list_of_states(),
		'#default_value' => $proposal_data->state,
		'#validated' => TRUE,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City'),
		'#options' => _df_list_of_cities(),
		'#default_value' => $proposal_data->city,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 30,
		'#maxlength' => 6,
		'#default_value' => $proposal_data->pincode,
		'#attributes' => array(
			'placeholder' => 'Insert pincode of your city/ village....'
		)
	);
	$form['category'] = array(
		'#type' => 'select',
		'#title' => t('Category'),
		'#default_value' => $proposal_data->category,
		'#required' => TRUE,
		'#options' => _soul_list_of_category()
		/*'#markup' => $proposal_data->category,*/
		
	);
	$form['project_guide_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the faculty member of your Institution, if any, who helped you with this project '),
		'#size' => 250,
		'#default_value' => $proposal_data->project_guide_name,
		'#attributes' => array(
			'placeholder' => t('Enter full name of faculty member')
		),
		'#maxlength' => 250
	);
	$form['project_guide_department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department of the faculty member of your Institution, if any, who helped you with this project '),
		'#size' => 250,
		'#default_value' => $proposal_data->project_guide_department,
		'#attributes' => array(
			'placeholder' => t('Enter department name of faculty member')
		),
		'#maxlength' => 250
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email id of the faculty member of your Institution, if any, who helped you with this project'),
		'#size' => 30,
		'#default_value' => $proposal_data->project_guide_email_id,
		'#attributes' => array(
			'placeholder' => t('Enter Email id of the faculty member')
		),
	);
	$form['project_guide_university'] = array(
		'#type' => 'textfield',
		'#title' => t('Enter University name of faculty member'),
		'#default_value' => $proposal_data->project_guide_university,
		'#size' => 80,
		'#maxlength' => 200,
		
		
	);
	// $form['sub_category'] = array(
	// 	'#type' => 'select',
	// 	'#default_value' => $proposal_data->sub_category,
	// 	'#title' => t('Sub Category'),
	// 	'#required' => TRUE,
	// 	'#options' => _soul_list_of_sub_category(),
	// 	'#states' => array(
	// 		'visible' => array(
	// 			':input[name="category"]' => array(
	// 				'value' => '3D Modeling Project'
	// 			)
	// 		)
	// 	)
	// );
	// $software_version_options =  _soul_list_of_software_version();
	// $form['software_version'] = array(
	// 	'#type' => 'select',
	// 	'default_value' => $software_versions,
	// 	'#title' => t('Software Version'),
	// 	'#options' => $software_version_options,
	// 	'#required' => TRUE,
	// 	'#ajax' => array(
    //     	'callback' => 'ajax_solver_used_callback',
    //     ),
	// );
	// $software_version_id = isset($form_state['values']['software_version']) ? $form_state['values']['software_version'] : key($software_version_options);
	// $form['software_version_no'] = array(
	// 	'#type' => 'select',
	// 	'#default_value' => $proposal_data->software_version_no,
	// 	'#title' => t('Software Version No'),
	// 	'#options' => _soul_list_of_software_version_details($software_version_id),
		
	// 	'#prefix' => '<div id="ajax-solver-replace">',
	// 	'#suffix' => '</div>',
	// 	'#required' => TRUE,
	// 	'#tree' => TRUE,
	// 	'#validated' => TRUE
	// );
	// /*$form['other_software_version_no'] = array(
	// 	'#type' => 'item',
	// 	'#default_value' => $proposal_data->other_software_version_no,
	// 	'#title' => t('Other Software Version No'),
	// 	'#size' => 100,
	// 	'#attributes' => array(
	// 		'placeholder' => t('Enter your answer')
	// 	),
	// 	'#states' => array(
	// 		'visible' => array(
	// 			':input[name="software_version_no"]' => array(
	// 				'value' => 'Another'
	// 			)
	// 		)
	// 	)
	// );*/
	// $form['second_software'] = array(
	// 	'#type' => 'item',
	// 	'#default_value' => $second_software,
	// 	'#title' => t('Second Software Version'),
	// 	'#options' => _soul_list_of_second_software_version()
	// );
	// if ($proposal_data->is_ncert_book == 'Yes') {
		$form['is_ncert_book'] = array(
			'#type' => 'radios',
			'#title' => t('Does the content of this project match any chapter  of a textbook'),
			'#options' => array(
				'Yes' => 'Yes',
				'No' => 'No'
			),
			'#required' => TRUE,
			'#default_value' => $proposal_data->is_ncert_book,

		);
		$form['preference1'] = array(
			'#type' => 'fieldset',
			'#title' => t('Details of Textbook'),
			'#collapsible' => TRUE,
			'#collapsed' => FALSE,
			'#states' => array(
				'visible' => array(
					':input[name="is_ncert_book"]' => array(
						'value' => 'Yes'
					)
				)
			)
		);
		$form['preference1']['book1'] = array(
			'#type' => 'textfield',
			'#title' => t('Title of the book'),
			'#size' => 30,
			'#maxlength' => 100,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#default_value' => $book_data->book,

		);
		$form['preference1']['author1'] = array(
			'#type' => 'textfield',
			'#title' => t('Author Name'),
			'#size' => 30,
			'#maxlength' => 100,
			'#default_value' => $book_data->author,

		);
		$form['preference1']['isbn1'] = array(
			'#type' => 'textfield',
			'#title' => t('ISBN No'),
			'#size' => 30,
			'#maxlength' => 25,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#default_value' => $book_data->isbn,

		);
		$form['preference1']['publisher1'] = array(
			'#type' => 'textfield',
			'#title' => t('Publisher & Place'),
			'#size' => 30,
			'#maxlength' => 50,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#default_value' => $book_data->publisher,

		);
		$form['preference1']['edition1'] = array(
			'#type' => 'textfield',
			'#title' => t('Edition'),
			'#size' => 4,
			'#maxlength' => 2,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#default_value' => $book_data->edition,

		);
		$form['preference1']['book_year'] = array(
			'#type' => 'textfield',
			'#title' => t('Year of publication'),
			'#size' => 4,
			'#maxlength' => 4,
			'#required' => TRUE,
			'#validated' => TRUE,
			'#default_value' => $book_data->book_year,

		);

	$form['year_of_study'] = array(
		'#type' => 'select',
		'#title' => t('The project is suitable for class (school education)/year of study(college education) '),
		// '#options' => '_tbc_list_of_main_categories()',
		'#options' =>_df_list_of_classes(),
		// '#default_value' => '-select-',
		'#tree' => TRUE,
		'#required' => TRUE,
		'#default_value' => $proposal_data->year_of_study,
		
	);
	$form['project_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Title of the Science and Concept Map Project'),
		'#size' => 75,
		'#required' => TRUE,
		'#default_value' => $proposal_data->project_title
	);
	$form['description'] = array(
		'#type' => 'textarea',
		'#default_value' => $proposal_data->description,
		'#title' => t('Project Description'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter Project Description')
		),
		'#description' => t('Minimum character limit is 250 and Maximum character limit is 700'),
		'#required' => TRUE
	);
	// SAMPLE FILE//
	$form['reference'] = array(
		'#type' => 'textfield',
		'#title' => t('Reference'),
		'#size' => 20,
		'#attributes' => array(
			'placeholder' => 'Links of must be provided....'
		),
		'#default_value' => $proposal_data->reference
	);
	$form['date_of_proposal'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date of Proposal'),
		'#default_value' => date("Y-m-d H:i:s"),
		'#date_format' => 'd M Y',
		'#disabled' => TRUE,
		'#date_label_position' => ''
	);
	// $form['expected_date_of_completion'] = array(
	// 	'#type' => 'date_popup',
	// 	'#title' => t('Expected Date of Completion'),
	// 	'#date_label_position' => '',
	// 	'#description' => '',
	// 	'#default_value' => '',
	// 	'#date_format' => 'd-M-Y',
		
	// 	'#date_year_range' => '0 : +1',
	// 	'#datepicker_options' => array('maxDate' => 31, 'minDate' => 0),
	// 	'#required' => TRUE
	// );
	$form['delete_proposal'] = array(
		'#type' => 'checkbox',
		'#title' => t('Delete Proposal')
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	// @FIXME
// l() expects a Url object, created from a route name or external URI.
// $form['cancel'] = array(
// 		'#type' => 'item',
// 		'#markup' => l(t('Cancel'), 'science-and-concept-map-project/manage-proposal')
// 	);

	return $form;
}

function science_and_concept_map_proposal_edit_form_validate($form, &$form_state)
{
	//Validation for project title
	$form_state['values']['project_title'] = trim($form_state['values']['project_title']);
	if ($form_state['values']['project_title'] != '')
	{
		if (strlen($form_state['values']['project_title']) > 250)
		{
			form_set_error('project_title', t('Maximum charater limit is 250 charaters only, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['project_title']) < 10)
		{
			form_set_error('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('project_title', t('Project title shoud not be empty'));
	}
	if(preg_match('/[\^£$%&*()}{@#~?><>.:;`|=+¬]/', $form_state['values']['project_title']))
	{
		form_set_error('project_title', t('Special characters are not allowed for Project Title'));
	} 
		return $form_state;
}


function science_and_concept_map_proposal_edit_form_submit($form, &$form_state)
{
	$user = \Drupal::currentUser();
	/* get current proposal */
	$proposal_id = (int) arg(3);
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	if ($proposal_q)
	{
		if ($proposal_data = $proposal_q->fetchObject())
		{
			/* everything ok */
		} //$proposal_data = $proposal_q->fetchObject()
		else
		{
			\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
			return;
		}
	} //$proposal_q
	else
	{
		\Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
		drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
	}
	/* delete proposal */
	if ($form_state['values']['delete_proposal'] == 1)
	{
		/* sending email */
		$user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
		$email_to = $user_data->mail;
		$from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
		$bcc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
		$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
		$params['science_and_concept_map_proposal_deleted']['proposal_id'] = $proposal_id;
		$params['science_and_concept_map_proposal_deleted']['user_id'] = $proposal_data->uid;
		$params['science_and_concept_map_proposal_deleted']['headers'] = array(
			'From' => $from,
			'MIME-Version' => '1.0',
			'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
			'Content-Transfer-Encoding' => '8Bit',
			'X-Mailer' => 'Drupal',
			'Cc' => $cc,
			'Bcc' => $bcc
		);
		if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_deleted', $email_to, user_preferred_language($user), $params, $from, TRUE))
			\Drupal::messenger()->addError('Error sending email message.');
		\Drupal::messenger()->addStatus(t('soul science-and-concept-map-project proposal has been deleted.'));
		if (rrmdir_project($proposal_id) == TRUE)
		{
			$query_book = \Drupal::database()->delete('soul_science_and_concept_map_textbook_details');
			$query_book->condition('proposal_id', $proposal_id);
			$num_book_deleted = $query_book->execute();
			$query = \Drupal::database()->delete('soul_science_and_concept_map_proposal');
			$query->condition('id', $proposal_id);
			$num_deleted = $query->execute();
			\Drupal::messenger()->addStatus(t('Proposal Deleted'));
			drupal_goto('science-and-concept-map-project/manage-proposal');
		return;
		} //rrmdir_project($proposal_id) == TRUE
	} //$form_state['values']['delete_proposal'] == 1
	/* update proposal */
	$v = $form_state['values'];
	$project_title = $v['project_title'];
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$directory_names = _scmp_dir_name($project_title, $proposar_name);
	if (DF_RenameDir($proposal_id, $directory_names))
	{
		$directory_name = $directory_names;
	} //LM_RenameDir($proposal_id, $directory_names)
	else
	{
		return;
	}
	$str = substr($proposal_data->abstractfilepath,strrpos($proposal_data->abstractfilepath, '/'));
	$resource_file =ltrim($str, '/');
	$abstractfilepath = $directory_name . '/' . $resource_file;
	$query = "UPDATE soul_science_and_concept_map_proposal SET 
				name_title=:name_title,
				contributor_name=:contributor_name,
				university=:university,
				city=:city,
				pincode=:pincode,
				project_guide_name=:project_guide_name,
    			project_guide_department=:project_guide_department,
    			project_guide_email_id=:project_guide_email_id,
				project_guide_university=:project_guide_university,
				state=:state,
				project_title=:project_title,
				description=:description,
				reference=:reference,
				directory_name=:directory_name ,
				abstractfilepath=:abstractfilepath,
				year_of_study=:year_of_study,
				is_ncert_book=:is_ncert_book
				WHERE id=:proposal_id";
	$args = array(
		':name_title' => $v['name_title'],
		':contributor_name' => $v['contributor_name'],
		':university' => $v['university'],
		':city' => $v['city'],
		':pincode' => $v['pincode'],
		":project_guide_name" => trim($v['project_guide_name']),
		":project_guide_department" =>trim($v['project_guide_department']),
		":project_guide_email_id" => trim($v['project_guide_email_id']),
		":project_guide_university" =>trim(($v['project_guide_university'])),
		':state' => $v['all_state'],
		':project_title' => $project_title,
		':description' => $v['description'],
		':reference' => $v['reference'],
		':directory_name' => $directory_name,
		':abstractfilepath' => $abstractfilepath,
		':proposal_id' => $proposal_id,
		":is_ncert_book" => $v['is_ncert_book'],
		":year_of_study" => trim($v['year_of_study'])
		
	);
	$result = \Drupal::database()->query($query, $args);
	$query_book =" UPDATE soul_science_and_concept_map_textbook_details SET
				book=:book, 
				author=:author,
				isbn =:isbn, 
				publisher=:publisher, 
				edition=:edition, 
				book_year=:book_year
				WHERE proposal_id=:proposal_id
	";
	$args_book=array(":proposal_id" => $proposal_id,
	":book" => trim(ucwords(strtolower($form_state['values']['book1']))),
	":author" => trim(ucwords(strtolower($form_state['values']['author1']))),
	":isbn" => trim($form_state['values']['isbn1']),
	":publisher" => trim(ucwords(strtolower($form_state['values']['publisher1']))),
	":edition" => trim($form_state['values']['edition1']),
	":book_year" => trim($form_state['values']['book_year'])
	);
	$result_books= \Drupal::database()->query($query_book, $args_book);
	\Drupal::messenger()->addStatus(t('Proposal Updated'));
}
