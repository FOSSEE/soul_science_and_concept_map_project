<?php
// $Id$
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Completed
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes


is_completed
0 - in progress
1 - Completed
Tables :
science_and_concept_map_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
function science_and_concept_map_proposal_form($form, &$form_state, $no_js_use = FALSE)
{
	$user = \Drupal::currentUser();
	/************************ start approve book details ************************/
	if ($user->uid == 0)
	{
		$msg = \Drupal::messenger()->addError(t('It is mandatory to ' . \Drupal\Core\Link::fromTextAndUrl('login', \Drupal\Core\Url::fromRoute('user.page')) . ' on this website to access the science and concept map proposal form. If you are new user please create a new account first.'));
		//drupal_goto('soul-science-and-concept-map-project');
		drupal_goto('user/login', array('query' => drupal_get_destination()));
		return $msg;
	} //$user->uid == 0
	$query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
	$query->fields('soul_science_and_concept_map_proposal');
	$query->condition('uid', $user->uid);
	$query->orderBy('id', 'DESC');
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if ($proposal_data)
	{
		if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1)
		{
			\Drupal::messenger()->addStatus(t('We have already received your proposal.'));
			drupal_goto('');
			return;
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);
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
		'#required' => TRUE
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#maxlength' => 250,
		'#required' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#maxlength' => 250
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('School/Institute/University/Organisation'),
		'#size' => 80,
		'#maxlength' => 200,
		'#attributes' => array(
			'placeholder' => 'Enter full name of your School/Institute/University/Organisation.... '
		),
		'#required' => TRUE
	);
	$form['department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department/Branch'),
		'#size' => 80,
		'#maxlength' => 200,
		'#attributes' => array(
			'placeholder' => 'Enter Department/Branch.... '
		)
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
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
	);
	$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
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
		'#attributes' => array(
			'placeholder' => t('Enter Pincode')
		),
		'#size' => 6
	);
	/***************************************************************************/
	$form['hr'] = array(
		'#type' => 'item',
		'#markup' => '<hr>'
	);
	$form['project_guide_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the faculty member of your Institution, if any, who helped you with this project '),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter full name of faculty member')
		),
		'#maxlength' => 250
	);
	$form['project_guide_department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department of the faculty member of your Institution, if any, who helped you with this project '),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter department name of faculty member')
		),
		'#maxlength' => 250
	);
	$form['project_guide_university'] = array(
		'#type' => 'textfield',
		'#title' => t('Enter University name of faculty member'),
		'#size' => 80,
		'#maxlength' => 200,
		'#attributes' => array(
			'placeholder' => 'Enter University name of faculty member'
		),
		
	);
	$form['project_guide_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email id of the faculty member of your Institution, if any, who helped you with this project'),
		'#size' => 30,
		'#attributes' => array(
			'placeholder' => t('Enter Email id of the faculty member')
		),
	);
	$form['options'] = array(
		'#type' => 'select',
		'#title' => t('How did you come to know about the science and concept map project'),
		'#options' => array(
			'Poster' => 'Poster',
			'Website' => 'Website',
			'Email' => 'Email',
			'Workshop' => 'Workshop',
			'others' => 'others'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['category'] = array(
		'#type' => 'select',
		'#title' => t('Select Category'),
		'#options' => _soul_list_of_category(),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['sub_category'] = array(
		'#type' => 'select',
		'#title' => t('Select Sub Category for 3D Modeling Project'),
		'#options' => _soul_list_of_sub_category(),
		'#states' => array(
			'visible' => array(
				':input[name="category"]' => array(
					'value' => '3D Modeling Project'
				)
			)
		)
	);
	$software_version_options =  _soul_list_of_software_version();
	$form['software_version'] = array(
		'#type' => 'select',
		'#title' => t('Select Software'),
		'#options' => $software_version_options,
		'#required' => TRUE,
		'#ajax' => array(
        	'callback' => 'ajax_solver_used_callback',
        ),
	);
	$software_version_id = isset($form_state['values']['software_version']) ? $form_state['values']['software_version'] : key($software_version_options);
	$form['software_version_no'] = array(
		'#type' => 'select',
		'#title' => t('Select the Software Version Number to be used'),
		'#options' => _soul_list_of_software_version_details($software_version_id),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax-solver-replace">',
		'#suffix' => '</div>',
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_software_version_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Enter your answer'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your answer')
		),
		'#states' => array(
			'visible' => array(
				':input[name="software_version_no"]' => array(
					'value' => 'Another'
				)
			)
		)
	);
	$form['second_software'] = array(
		'#type' => 'select',
		'#title' => t('Select Second Software (If applicable)'),
		'#options' => _soul_list_of_second_software_version()
	);
	/************** */
	$form['is_ncert_book'] = array(
		'#type' => 'radios',
		'#title' => t('Does the content of this project match any chapter  of a textbook'),
		'#options' => array(
			'Yes' => 'Yes',
			'No' => 'No'
		),
		'#required' => TRUE
	);
	/**************************************** */
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
		'#validated' => TRUE
	);
	$form['preference1']['author1'] = array(
		'#type' => 'textfield',
		'#title' => t('Author Name'),
		'#size' => 30,
		'#maxlength' => 100,
		// '#required' => TRUE
		//'#value' => $row1->author,
		//'#disabled' => ($row1->author?TRUE:FALSE),
	);
	$form['preference1']['isbn1'] = array(
		'#type' => 'textfield',
		'#title' => t('ISBN No'),
		'#size' => 30,
		'#maxlength' => 25,
		'#required' => TRUE,
		'#validated' => TRUE
		// '#required' => TRUE
		// '#value' => $row1->isbn,
		// '#disabled' => ($row1->isbn?TRUE:FALSE),
	);
	$form['preference1']['publisher1'] = array(
		'#type' => 'textfield',
		'#title' => t('Publisher & Place'),
		'#size' => 30,
		'#maxlength' => 50,
		'#required' => TRUE,
		'#validated' => TRUE
		// '#required' => TRUE
		//'#value' => $row1->publisher,
	);
	$form['preference1']['edition1'] = array(
		'#type' => 'textfield',
		'#title' => t('Edition'),
		'#size' => 4,
		'#maxlength' => 2,
		'#required' => TRUE,
		'#validated' => TRUE
		// '#required' => TRUE
		//'#value' => $row1->edition,
	);
	$form['preference1']['book_year'] = array(
		'#type' => 'textfield',
		'#title' => t('Year of publication'),
		'#size' => 4,
		'#maxlength' => 4,
		'#required' => TRUE,
		'#validated' => TRUE
		// '#required' => TRUE
		//'#value' => $row1->year,
	);
	$form['year_of_study'] = array(
		'#type' => 'select',
		'#title' => t('The project is suitable for class (school education)/year of study(college education) '),
		// '#options' => '_tbc_list_of_main_categories()',
		'#options' =>_df_list_of_classes(),
		//'#default_value' => '-select-',
		'#tree' => TRUE,
		'#required' => TRUE
	);
	/*************************************** */
	$form['project_title'] = array(
		'#type' => 'textfield',
		'#title' => t('Project Title'),
		'#size' => 100,
		'#maxlength' => 250,
		'#description' => t('Minimum character limit is 10 and Maximum character limit is 250'),
		'#attributes' => array(
			'placeholder' => t('Enter Project Title')
		),
		'#required' => TRUE
	);
	$form['description'] = array(
		'#type' => 'textarea',
		'#title' => t('Project Description'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter Project Description')
		),
		'#description' => t('Minimum character limit is 250 and Maximum character limit is 700'),
		'#required' => TRUE
	);	
	$form['samplefile'] = array(
		'#type' => 'fieldset',
		'#title' => t('Submit an abstract<span style="color:red">*</span>'),
		'#collapsible' => FALSE,
		'#collapsed' => FALSE
	);
	$form['samplefile']['abstractfilepath'] = array(
		'#type' => 'file',
		//'#title' => t('Upload circuit diagram'),
		'#size' => 48,
		'#description' => t('Upload filenames with allowed extensions only. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_abstract_upload_extensions') . '</span>'
	);
	$form['reference'] = array(
		'#type' => 'textfield',
		'#description' => t('The links to the documents or websites which are referenced while proposing this project.'),
		'#title' => t('Reference'),
		'#size' => 250,
		'#required' => TRUE,

		'#attributes' => array(
			'placeholder' => 'Enter reference'
		)

	);
	/*$form['fellowship'] = array(
		'#type' => 'radios',
		'#title' => t('If you are applying for FOSSEE Summer Fellowship 2018 - soul Screening level Task 1, select Yes.(To know more about the FOSSEE Summer Fellowship 2018 <a href="https://fossee.in/fellowship/" target="_blank">Click here</a>)'),
		'#options' => array(
			'1' => 'Yes',
			'2' => 'No'
		),
		'#required' => TRUE
	);*/

	$form['date_of_proposal'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date of Proposal'),
		'#default_value' => date("Y-m-d H:i:s"),
		'#date_format' => 'd M Y',
		'#disabled' => TRUE,
		'#date_label_position' => '',
	);
	$form['expected_date_of_completion'] = array(
		'#type' => 'date_popup',
		'#title' => t('Expected Date of Completion'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => '',
		'#date_format' => 'd-M-Y',
		//'#date_increment' => 0,
		//'#minDate' => '+0',
		'#date_year_range' => '0 : +1',
		'#datepicker_options' => array('maxDate' => 31, 'minDate' => 0),
		'#required' => TRUE
	);

	$form['term_condition'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('<a href="term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
		),
		'#required' => TRUE
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}

function ajax_solver_used_callback($form, $form_state)
{
	$commands = array();
	$software_version_id = isset($form_state['values']['software_version']) ? $form_state['values']['software_version'] : key($software_version_options);
	if($software_version_id != 7){
		$form['software_version_no']['#options'] = _soul_list_of_software_version_details($software_version_id);/*
		$form['software_version_no']['#required'] = TRUE;
		$form['software_version_no']['#validated'] = TRUE;*/
		$commands[] = ajax_command_replace('#ajax-solver-replace', \Drupal::service("renderer")->render($form['software_version_no']));
		$commands[] = ajax_command_html('#ajax-solver-text-replace', '');
	}
	else{
		$commands[] = ajax_command_html('#ajax-solver-replace', '');
		$commands[] = ajax_command_html('#ajax-solver-text-replace', \Drupal::service("renderer")->render($form['other_software_version_no']));
	}
	return array(
        '#type' => 'ajax',
        '#commands' => $commands
    );
}
function science_and_concept_map_proposal_form_validate($form, &$form_state)
{
	if(preg_match('/[\^£$%&*()}{@#~?><>.:;`|=_+¬]/', $form_state['values']['contributor_name']))
	{
		form_set_error('contributor_name', t('Special characters are not allowed for Contributor Name'));
	} 
	if (!preg_match('/^[0-9\ \+]{0,15}$/', $form_state['values']['contributor_contact_no']))
	{
    	form_set_error('contributor_contact_no', t('Invalid contact phone number'));
 	}
	if ($form_state['values']['term_condition'] == '1')
	{
		form_set_error('term_condition', t('Please check the terms and conditions'));
		// $form_state['values']['country'] = $form_state['values']['other_country'];
	} //$form_state['values']['term_condition'] == '1'

	if ($form_state['values']['country'] == 'Others')
	{
		if ($form_state['values']['other_country'] == '')
		{
			form_set_error('other_country', t('Enter country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_country'] == ''
		else
		{
			$form_state['values']['country'] = $form_state['values']['other_country'];
		}
		if ($form_state['values']['other_state'] == '')
		{
			form_set_error('other_state', t('Enter state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_state'] == ''
		else
		{
			$form_state['values']['all_state'] = $form_state['values']['other_state'];
		}
		if ($form_state['values']['other_city'] == '')
		{
			form_set_error('other_city', t('Enter city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_city'] == ''
		else
		{
			$form_state['values']['city'] = $form_state['values']['other_city'];
		}
	} //$form_state['values']['country'] == 'Others'
	else
	{
		if ($form_state['values']['country'] == '')
		{
			form_set_error('country', t('Select country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['country'] == ''
		if ($form_state['values']['all_state'] == '')
		{
			form_set_error('all_state', t('Select state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['all_state'] == ''
		if ($form_state['values']['city'] == '')
		{
			form_set_error('city', t('Select city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['city'] == ''
	}
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
	$form_state['values']['description'] = trim($form_state['values']['description']);
	if ($form_state['values']['description'] != '')
	{
		if (strlen($form_state['values']['description']) > 700)
		{
			form_set_error('description', t('Maximum charater limit is 700 charaters only, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) > 250
		else if (strlen($form_state['values']['description']) < 250)
		{
			form_set_error('description', t('Minimum charater limit is 250 charaters, please check the length of the description'));
		} //strlen($form_state['values']['project_title']) < 10
	} //$form_state['values']['project_title'] != ''
	else
	{
		form_set_error('description', t('Description shoud not be empty'));
	}
if (isset($_FILES['files']))
	{
		/* check if atleast one source or result file is uploaded */
		if (!($_FILES['files']['name']['abstractfilepath']))
			form_set_error('abstractfilepath', t('Please upload Abstract file.'));
		/* check for valid filename extensions */
		foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
		{
			if ($file_name)
			{
				/* checking file type */
				/*if (strstr($file_form_name, 'sample'))
					$file_type = 'S';
				else
					$file_type = 'U';
				
				/*switch ($file_type)
				{
					case 'S':
						$allowed_extensions_str = variable_get('textbook_companion_source_extensions', '');
						break;
				} *///$file_type
				$allowed_extensions_str = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_abstract_upload_extensions');
				$allowed_extensions = explode(',', $allowed_extensions_str);
				$fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
				$temp_extension = end($fnames);
				if (!in_array($temp_extension, $allowed_extensions))
					form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
				if ($_FILES['files']['size'][$file_form_name] <= 0)
					form_set_error($file_form_name, t('File size cannot be zero.'));
				/* check if valid file name */
				/*if (!textbook_companion_check_valid_filename($_FILES['files']['name'][$file_form_name]))
					form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));*/
			} //$file_name
		} //$_FILES['files']['name'] as $file_form_name => $file_name
	} 
	//     
	if ($form_state['values']['is_ncert_book'] == 'Yes') {
		$preference1 = $form_state['values'];
		if ($preference1['book1'] == '') {

			form_set_error('book1', t('Title of the book is required.'));

		}
		if ($preference1['isbn1'] == '') {

			form_set_error('isbn1', t('ISBN No is required.'));

		}
		if ($preference1['publisher1'] == '') {

			form_set_error('publisher1', t('Publisher & Place is required.'));

		}
		if ($preference1['edition1'] == '') {

			form_set_error('edition1', t('Edition is required.'));

		}
		if ($preference1['book_year'] == '') {

			form_set_error('book_year', t('Year of publication is required.'));

		}
	}
	return $form_state;
}
function science_and_concept_map_proposal_form_submit($form, &$form_state)
{
	$user = \Drupal::currentUser();
	$root_path = science_and_concept_map_path();
	if (!$user->uid)
	{
		\Drupal::messenger()->addError('It is mandatory to login on this website to access the proposal form');
		return;
	} //!$user->uid
	/* inserting the user proposal */
	$v = $form_state["values"];
	$project_title = trim($v['project_title']);
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$department = $v['department'];
	$options = $v['options'];
	$category = $v['category'];
	$software_version_no =$v['software_version_no'];
	$directory_name = _scmp_dir_name($project_title, $proposar_name);
	if ($category == '3D Modeling Project')
	{
		$sub_category = $v['sub_category'];
	}
	else
	{
		$sub_category = 'None';
	}

	if ($software_version_no == 'Another')
	{
		$other_software_version_no = $v['other_software_version_no'];
	}
	else
	{
		$other_software_version_no = 'None';
	}
	
	$result = "INSERT INTO {soul_science_and_concept_map_proposal} 
    (
    uid, 
    approver_uid,
    name_title, 
    contributor_name,
    contact_no,
    university,
    department,
    city, 
    state, 
    country,
    pincode,
    project_guide_name,
    project_guide_department,
    project_guide_email_id,
	project_guide_university,
    options,
    category,
    sub_category,
    project_title, 
    description,
    software_version,
    software_version_no,
    other_software_version_no,
    second_software,
    directory_name,
    approval_status,
    is_completed, 
    dissapproval_reason,
    creation_date, 
    approval_date,
    abstractfilepath,
    reference,
    expected_date_of_completion,
	is_ncert_book,
	year_of_study
    ) VALUES
    (
    :uid, 
    :approver_uid, 
    :name_title, 
    :contributor_name, 
    :contact_no,
    :university, 
    :department,
    :city,
    :state,  
    :country, 
    :pincode,
    :project_guide_name,
    :project_guide_department,
    :project_guide_email_id,
	:project_guide_university,
    :options,
    :category,
    :sub_category,
    :project_title, 
    :description,
    :software_version,
    :software_version_no,
    :other_software_version_no,
    :second_software,
    :directory_name,
    :approval_status,
    :is_completed, 
    :dissapproval_reason,
    :creation_date, 
    :approval_date,
    :abstractfilepath,
    :reference,
    :expected_date_of_completion,
	:is_ncert_book,
	:year_of_study
    )";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":contributor_name" => trim($v['contributor_name']),
		":contact_no" => $v['contributor_contact_no'],
		":university" => trim($v['university']),
		":department" => $v['department'],
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => $v['country'],
		":project_guide_name" => trim($v['project_guide_name']),
		":project_guide_department" =>trim($v['project_guide_department']),
		":project_guide_email_id" => trim($v['project_guide_email_id']),
		":project_guide_university" =>trim(($v['project_guide_university'])),
		":options" =>$v['options'],
		":category" => $category,
		":sub_category" => $sub_category,
		":project_title" => $v['project_title'],
		":description" => $v['description'],
		":software_version" =>$v['software_version'],		
		":software_version_no" =>$software_version_no,
		":other_software_version_no" => $other_software_version_no,
		":second_software" => $v['second_software'],
		":directory_name" => $directory_name,
		":approval_status" => 0,
		":is_completed" => 0,
		":dissapproval_reason" => "NULL",
		":creation_date" => time(),
		":approval_date" => 0,
		":expected_date_of_completion" => strtotime(date($v['expected_date_of_completion'])),
		":abstractfilepath" => "",
		":reference" => $v['reference'],
		":is_ncert_book" => $v['is_ncert_book'],
		":year_of_study" => trim($form_state['values']['year_of_study']),
		);
	//var_dump($args);die;
	//var_dump($result);die;
	$result1 = \Drupal::database()->query($result, $args, $result);
//var_dump($args);die;
$proposal_id = $result1;
	/* inserting first book preference */
	if ($form_state['values']['book1'])
	{
		$bk1 = trim($form_state['values']['book1']);
		$auth1 = trim($form_state['values']['author1']);
		$pref_id = NULL;
		// $directory_name = _dir_name($bk1, $auth1, $pref_id);
		$query = "INSERT INTO soul_science_and_concept_map_textbook_details
      (proposal_id, book, author, isbn, publisher, edition, book_year) VALUES (:proposal_id, :book, :author, :isbn, :publisher, :edition, :book_year)
	";
		$args = array(
			":proposal_id" => $proposal_id,
			":book" => trim(ucwords(strtolower($form_state['values']['book1']))),
			":author" => trim(ucwords(strtolower($form_state['values']['author1']))),
			":isbn" => trim($form_state['values']['isbn1']),
			":publisher" => trim(ucwords(strtolower($form_state['values']['publisher1']))),
			":edition" => trim($form_state['values']['edition1']),
			":book_year" => trim($form_state['values']['book_year']),
		);
		$result = \Drupal::database()->query($query, $args, $query);
		if (!$result)
		{
			\Drupal::messenger()->addError(t('Error receiving your first book preference.'));
		} //!$result
	} //$form_state['values']['book1']

	$dest_path = $directory_name . '/';
	$dest_path1 = $root_path . $dest_path;
	//var_dump($dest_path1);die;	
	if (!is_dir($root_path . $dest_path))
		mkdir($root_path . $dest_path);
	/* uploading files */
	foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	{
		if ($file_name)
		{
			/* checking file type */
			//$file_type = 'S';
			if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				 \Drupal::messenger()->addError(t("Error uploading file. File !filename already exists.", array('!filename' => $_FILES['files']['name'][$file_form_name])));
				//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
			} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			/* uploading file */
			if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
			{
				$query = "UPDATE {soul_science_and_concept_map_proposal} SET abstractfilepath = :abstractfilepath WHERE id = :id";
				$args = array(
					":abstractfilepath" => $dest_path . $_FILES['files']['name'][$file_form_name],
					":id" => $result1
				);
				
				$updateresult = \Drupal::database()->query($query, $args);
				//var_dump($args);die;

				\Drupal::messenger()->addStatus($file_name . ' uploaded successfully.');
			} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
			else
			{
				\Drupal::messenger()->addError('Error uploading file : ' . $dest_path . '/' . $file_name);
			}
		} //$file_name
	} //$_FILES['files']['name'] as $file_form_name => $file_name
	if (!$result1)
	{
		\Drupal::messenger()->addError(t('Error receiving your proposal. Please try again.'));
		return;
	} //!$proposal_id
	/* sending email */
	$email_to = $user->mail;
	$form = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
	$bcc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
	$cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
	$params['science_and_concept_map_proposal_received']['result1'] = $result1;
	$params['science_and_concept_map_proposal_received']['user_id'] = $user->uid;
	$params['science_and_concept_map_proposal_received']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_received', $email_to, user_preferred_language($user), $params, $form, TRUE))
		\Drupal::messenger()->addError('Error sending email message.');
	\Drupal::messenger()->addStatus(t('We have received your soul science and concept map proposal. We will get back to you soon.'));
	drupal_goto('');
}
