<?php

/**
 * @file
 * Contains \Drupal\science_and_concept_map\Form\ScienceAndConceptMapProposalApprovalForm.
 */

namespace Drupal\science_and_concept_map\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class ScienceAndConceptMapProposalApprovalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'science_and_concept_map_proposal_approval_form';
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $query = \Drupal::database()->select('soul_science_and_concept_map_textbook_details');
    $query ->fields('soul_science_and_concept_map_textbook_details');
    $query->condition('proposal_id', $proposal_id);
    $book_q = $query->execute();
    $book_data = $book_q->fetchObject();
    $query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
    $query->fields('soul_science_and_concept_map_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        drupal_goto('science-and-concept-map-project/manage-proposal');
        return;
      }
    } //$proposal_q
    else {
      \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      drupal_goto('science-and-concept-map-project/manage-proposal');
      return;
    }
    if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "") {
      $project_guide_name = "Not Entered";
    } //$proposal_data->project_guide_name == NULL
    else {
      $project_guide_name = $proposal_data->project_guide_name;
    }
    if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "") {
      $project_guide_email_id = "Not Entered";
    } //$proposal_data->project_guide_email_id == NULL
    else {
      $project_guide_email_id = $proposal_data->project_guide_email_id;
    }
    if ($proposal_data->project_guide_department == "NULL" || $proposal_data->project_guide_department == "") {
      $project_guide_department = "Not Entered";
    } //$proposal_data->project_guide_email_id == NULL
    else {
      $project_guide_department = $proposal_data->project_guide_department;
    }
    if ($proposal_data->project_guide_university == "NULL" || $proposal_data->project_guide_university == "") {
      $project_guide_university = "Not Entered";
    } //$proposal_data->project_guide_email_id == NULL
    else {
      $project_guide_university = $proposal_data->project_guide_university;
    }
    $query = \Drupal::database()->select('soul_science_and_concept_map_software_version');
    $query->fields('soul_science_and_concept_map_software_version');
    $query->condition('id', $proposal_data->software_version);
    $software_version_data = $query->execute()->fetchObject();
    if (!$software_version_data) {
      $software_versions = 'NA';
    }
    else {
      $software_versions = $software_version_data->software_versions;
    }
    if ($proposal_data->second_software == "NULL" || $proposal_data->second_software == "") {
      $second_software = "Not Entered";

    } //$proposal_data->city == NULL
    else {
      $second_software = $proposal_data->second_software;
    }
    // @FIXME
    // l() expects a Url object, created from a route name or external URI.
    // $form['contributor_name'] = array(
    // 		'#type' => 'item',
    // 		'#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
    // 		'#title' => t('Contributor name')
    // 	);

    $form['student_email_id'] = [
      '#title' => t('Student Email'),
      '#type' => 'item',
      '#markup' => \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid)->mail,
      // '#title' => t('Email')
    ];
    $form['contributor_contact_no'] = [
      '#title' => t('Contact No.'),
      '#type' => 'item',
      '#markup' => $proposal_data->contact_no,
    ];
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
    $form['university'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->university,
      '#title' => t('University/Institute'),
    ];
    $form['department'] = [
      '#type' => 'item',
      '#title' => t('Department/Branch'),
      '#markup' => $proposal_data->department,
    ];
    $form['country'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->country,
      '#title' => t('Country'),
    ];
    if ($proposal_data->country == 'others') {
      $form['other_country'] = [
        '#type' => 'item',
        '#title' => t('Other than India'),
        '#markup' => $proposal_data->other_country,
      ];
    }
    $form['all_state'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->state,
      '#title' => t('State'),
    ];
    $form['city'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->city,
      '#title' => t('City'),
    ];
    $form['pincode'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->pincode,
      '#title' => t('Pincode/Postal code'),
    ];

    $form['project_guide_name'] = [
      '#type' => 'item',
      '#title' => t('Name of the faculty member of your Institution, if any, who helped you with this project '),
      '#markup' => $project_guide_name,
    ];
    $form['project_guide_department'] = [
      '#type' => 'item',
      '#title' => t('Department of the faculty member of your Institution, if any, who helped you with this project '),
      '#markup' => $project_guide_department,
    ];
    $form['project_guide_email_id'] = [
      '#type' => 'item',
      '#title' => t('Email id of the faculty member of your Institution, if any, who helped you with this project'),
      '#markup' => $project_guide_email_id,
    ];
    $form['project_guide_university'] = [
      '#type' => 'item',
      '#title' => t('University name of the faculty member of your Institution, if any, who helped you with this project'),
      '#markup' => $project_guide_university,
    ];
    $form['options'] = [
      '#type' => 'item',
      '#title' => t('How did you come to know about the Science and Concept Map project'),
      '#markup' => $proposal_data->options,
    ];
    $form['category'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->category,
      '#title' => t('Category'),
    ];
    $form['sub_category'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->sub_category,
      '#title' => t('Sub Category'),
    ];
    $form['software_version'] = [
      '#type' => 'item',
      '#markup' => $software_versions,
      '#title' => t('Software Version'),
    ];
    $form['software_version_no'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->software_version_no,
      '#title' => t('Software Version No'),
    ];
    $form['other_software_version_no'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->other_software_version_no,
      '#title' => t('Other Software Version No'),
    ];
    $form['second_software'] = [
      '#type' => 'item',
      '#markup' => $second_software,
      '#title' => t('Second Software Version'),
    ];
    if ($proposal_data->is_ncert_book == 'Yes') {
      $form['book_name'] = [
        '#type' => 'item',
        '#markup' => $book_data->book,
        '#title' => t('Book name'),
      ];
      $form['author_name'] = [
        '#type' => 'item',
        '#markup' => $book_data->author,
        '#title' => t('Author name'),
      ];
      $form['isbn_no'] = [
        '#type' => 'item',
        '#markup' => $book_data->isbn,
        '#title' => t('ISBN no.'),
      ];
      $form['publisher'] = [
        '#type' => 'item',
        '#markup' => $book_data->publisher,
        '#title' => t('Publisher and place'),
      ];
      $form['edition'] = [
        '#type' => 'item',
        '#markup' => $book_data->edition,
        '#title' => t('Edition of a book'),
      ];
      $form['book_year'] = [
        '#type' => 'item',
        '#markup' => $book_data->book_year,
        '#title' => t('Year of publication'),
      ];
    }
    $form['year_of_study'] = [
      '#type' => 'item',
      '#markup' => $book_data->year_of_study,
      '#title' => t('The project is suitable for class (school education)/year of study(college education)'),
    ];
    $form['project_title'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->project_title,
      '#title' => t('Title of the Science and Concept Map Project'),
    ];
    $form['description'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->description,
      '#title' => t('Description of the Science and Concept Map Project'),
    ];
    $form['reference'] = [
      '#type' => 'item',
      '#markup' => $proposal_data->reference,
      '#title' => t('Reference'),
    ];
    if (($proposal_data->abstractfilepath != "") && ($proposal_data->abstractfilepath != 'NULL')) {
      $str = substr($proposal_data->abstractfilepath, strrpos($proposal_data->abstractfilepath, '/'));
      $resource_file = ltrim($str, '/');

      // @FIXME
      // l() expects a Url object, created from a route name or external URI.
      // $form['abstractfilepath'] = array(
      // 			'#type' => 'item',
      // 			'#title' => t('Abstract file '),
      // 			'#markup' => l($resource_file, 'science-and-concept-map-project/download/abstract-file/' . $proposal_id) . ""
      // 		);

    } //$proposal_data->user_defined_compound_filepath != ""
    else {
      $form['abstractfilepath'] = [
        '#type' => 'item',
        '#title' => t('Abstract file '),
        '#markup' => "Not uploaded<br><br>",
      ];
    }
    $form['approval'] = [
      '#type' => 'radios',
      '#title' => t('Soul Science and Concept Map Proposal'),
      '#options' => [
        '1' => 'Approve',
        '2' => 'Disapprove',
      ],
      '#required' => TRUE,
    ];
    $form['message'] = [
      '#type' => 'textarea',
      '#title' => t('Reason for disapproval'),
      '#attributes' => [
        'placeholder' => t('Enter reason for disapproval in minimum 30 characters '),
        'cols' => 50,
        'rows' => 4,
      ],
      '#states' => [
        'visible' => [
          ':input[name="approval"]' => [
            'value' => '2'
            ]
          ]
        ],
    ];
    $form['submit'] = [
      '#type' => 'submit',
      '#value' => t('Submit'),
    ];
    // @FIXME
    // l() expects a Url object, created from a route name or external URI.
    // $form['cancel'] = array(
    // 		'#type' => 'item',
    // 		'#markup' => l(t('Cancel'), 'science-and-concept-map-project/manage-proposal')
    // 	);

    return $form;
  }

  public function validateForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    if ($form_state->getValue(['approval']) == 2) {
      if ($form_state->getValue(['message']) == '') {
        $form_state->setErrorByName('message', t('Reason for disapproval could not be empty'));
      } //$form_state['values']['message'] == ''
    } //$form_state['values']['approval'] == 2
  }

  public function submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $user = \Drupal::currentUser();
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
    $query->fields('soul_science_and_concept_map_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
      if ($proposal_data = $proposal_q->fetchObject()) {
        /* everything ok */
      } //$proposal_data = $proposal_q->fetchObject()
      else {
        \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
        drupal_goto('science-and-concept-map-project/manage-proposal');
        return;
      }
    } //$proposal_q
    else {
      \Drupal::messenger()->addError(t('Invalid proposal selected. Please try again.'));
      drupal_goto('science-and-concept-map-project/manage-proposal');
      return;
    }
    if ($form_state->getValue(['approval']) == 1) {
      $query = "UPDATE {soul_science_and_concept_map_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 1 WHERE id = :proposal_id";
      $args = [
        ":uid" => $user->uid,
        ":date" => time(),
        ":proposal_id" => $proposal_id,
      ];
      \Drupal::database()->query($query, $args);
      /* sending email */
      $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
      $email_to = $user_data->mail;
      $from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
      $bcc = $user->mail . ', ' . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
      $cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
      $params['science_and_concept_map_proposal_approved']['proposal_id'] = $proposal_id;
      $params['science_and_concept_map_proposal_approved']['user_id'] = $proposal_data->uid;
      $params['science_and_concept_map_proposal_approved']['headers'] = [
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc,
      ];
      if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_approved', $email_to, language_default(), $params, $from, TRUE)) {
        \Drupal::messenger()->addError('Error sending email message.');
      }
      \Drupal::messenger()->addStatus('soul science-and-concept-map proposal No. ' . $proposal_id . ' approved. User has been notified of the approval.');
      drupal_goto('science-and-concept-map-project/manage-proposal');
      return;
    } //$form_state['values']['approval'] == 1
    else {
      if ($form_state->getValue(['approval']) == 2) {
        $query = "UPDATE {soul_science_and_concept_map_proposal} SET approver_uid = :uid, approval_date = :date, approval_status = 2, dissapproval_reason = :dissapproval_reason WHERE id = :proposal_id";
        $args = [
          ":uid" => $user->uid,
          ":date" => time(),
          ":dissapproval_reason" => $form_state->getValue(['message']),
          ":proposal_id" => $proposal_id,
        ];
        $result = \Drupal::database()->query($query, $args);
        /* sending email */
        $user_data = \Drupal::entityTypeManager()->getStorage('user')->load($proposal_data->uid);
        $email_to = $user_data->mail;
        $from = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_from_email');
        $bcc = $user->mail . ', ' . \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_emails');
        $cc = \Drupal::config('science_and_concept_map.settings')->get('science_and_concept_map_cc_emails');
        $params['science_and_concept_map_proposal_disapproved']['proposal_id'] = $proposal_id;
        $params['science_and_concept_map_proposal_disapproved']['user_id'] = $proposal_data->uid;
        $params['science_and_concept_map_proposal_disapproved']['headers'] = [
          'From' => $from,
          'MIME-Version' => '1.0',
          'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
          'Content-Transfer-Encoding' => '8Bit',
          'X-Mailer' => 'Drupal',
          'Cc' => $cc,
          'Bcc' => $bcc,
        ];
        if (!drupal_mail('science_and_concept_map', 'science_and_concept_map_proposal_disapproved', $email_to, language_default(), $params, $from, TRUE)) {
          \Drupal::messenger()->addError('Error sending email message.');
        }
        \Drupal::messenger()->addError('soul science and concept map proposal No. ' . $proposal_id . ' dis-approved. User has been notified of the dis-approval.');
        drupal_goto('science-and-concept-map-project/manage-proposal');
        return;
      }
    } //$form_state['values']['approval'] == 2
  }

}
?>
