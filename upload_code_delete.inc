<?php

/******************************************************************************/
/***************************** DELETE CODE ************************************/
/******************************************************************************/

function soul_science_and_concept_map_upload_code_delete()
{
  $user = \Drupal::currentUser();

  $root_path = science_and_concept_map_path();
  $solution_id = (int)arg(3);

  /* check solution */
 // $solution_q = db_query("SELECT * FROM {soul_science_and_concept_map_solution} WHERE id = %d LIMIT 1", $solution_id);
  $query = \Drupal::database()->select('soul_science_and_concept_map_solution');
              $query->fields('soul_science_and_concept_map_solution');
              $query->condition('id', $solution_id);
              $query->range(0, 1);
              $solution_q = $query->execute();
  $solution_data = $solution_q->fetchObject();
  if (!$solution_data)
  {
    \Drupal::messenger()->addError('Invalid solution.');
    drupal_goto('science-and-concept-map-project/code');
    return;
  }
  if ($solution_data->approval_status != 0)
  {
    \Drupal::messenger()->addError('You cannnot delete a solution after it has been approved. Please contact site administrator if you want to delete this solution.');
    drupal_goto('science-and-concept-map-project/code');
    return;
  }

  //$experiment_q = db_query("SELECT * FROM {soul_science_and_concept_map_experiment} WHERE id = %d LIMIT 1", $solution_data->experiment_id);
  $query = \Drupal::database()->select('soul_science_and_concept_map_experiment');
            $query->fields('soul_science_and_concept_map_experiment');
            $query->condition('id', $solution_data->experiment_id);
            $query->range(0, 1);
            $experiment_q = $query->execute();

  $experiment_data = $experiment_q->fetchObject();
  if (!$experiment_data)
  {
    \Drupal::messenger()->addError('You do not have permission to delete this solution.');
    drupal_goto('science-and-concept-map-project/code');
    return;
  }

  //$proposal_q = db_query("SELECT * FROM {soul_science_and_concept_map_proposal} WHERE id = %d AND solution_provider_uid = %d LIMIT 1", $experiment_data->proposal_id, $user->uid);
  $query = \Drupal::database()->select('soul_science_and_concept_map_proposal');
              $query->fields('soul_science_and_concept_map_proposal');
              $query->condition('id', $experiment_data->proposal_id);
              $query->condition('solution_provider_uid', $user->uid);
              $query->range(0, 1);
              $proposal_q = $query->execute();
  $proposal_data = $proposal_q->fetchObject();
  if (!$proposal_data)
  {
    \Drupal::messenger()->addError('You do not have permission to delete this solution.');
    drupal_goto('science-and-concept-map-project/code');
    return;
  }

  /* deleting solution files */
  if (soul_science_and_concept_map_delete_solution($solution_data->id))
  {
    \Drupal::messenger()->addStatus('Solution deleted.');

    /* sending email */
    $email_to = $user->mail;

    // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $from = variable_get('soul_science_and_concept_map_from_email', '');

    // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $bcc= variable_get('soul_science_and_concept_map_emails', '');

    // @FIXME
// // @FIXME
// // This looks like another module's variable. You'll need to rewrite this call
// // to ensure that it uses the correct configuration object.
// $cc=variable_get('soul_science_and_concept_map_cc_emails', '');
  

    $param['solution_deleted_user']['lab_title'] = $proposal_data->lab_title;
    $param['solution_deleted_user']['experiment_title'] = $experiment_data->title;
    $param['solution_deleted_user']['solution_number'] = $solution_data->code_number;
    $param['solution_deleted_user']['solution_caption'] = $solution_data->caption;
    $param['solution_deleted_user']['user_id'] = $user->uid;
    $param['solution_deleted_user']['headers']=array('From'=>$from,'MIME-Version'=> '1.0',
    			'Content-Type'=> 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
    			'Content-Transfer-Encoding' => '8Bit',
    			'X-Mailer'=> 'Drupal','Cc' => $cc, 'Bcc' => $bcc);

    if (!drupal_mail('soul_science_and_concept_map', 'solution_deleted_user', $email_to, language_default(), $param , $from , TRUE))
      \Drupal::messenger()->addError('Error sending email message.');
  } else {
    \Drupal::messenger()->addStatus('Error deleting example.');
  }

  drupal_goto('science-and-concept-map-project/code');
  return;
}