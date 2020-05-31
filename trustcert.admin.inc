<?php

function trustcert_admin_list() {
	$tc_rows = array();
	$tc_list = db_select('trustcert','t')
		->fields('t',array('id','host'))
		->orderBy('id','ASC')
		->execute()
		->fetchAllAssoc('id');
	$tc_oper = array(
		'data' => array(
			'#theme' => 'links',
			'#attributes' => array(
				'class' => array('links', 'inline', 'nowrap')
			),
			'#links' => array(
				'delete' => array(
					'title' => t('Delete')
				)
			)
		)
	);
	foreach($tc_list as $tc) {
		$tc_oper['data']['#links']['delete']['href'] =
			TRUSTCERT_ADMIN_URI . '/delete/' . $tc->id;
		$tc_rows[] = array(
			'data' => array(
				'id' => $tc->id,'host' => $tc->host,'operations' => $tc_oper
			)
		);
	}
	return theme('table',array(
		'empty' => t(
			'No records available. <a href="@link">Add host</a>.',
			array('@link' => url(TRUSTCERT_ADMIN_URI . '/add'))),
		'header' => array(
			'#',t('Host'),t('Operation')
		),
		'rows' => $tc_rows,
	));
}

function trustcert_admin_form($form, &$form_state) {
	$form['tc_host'] = array(
		'#type' => 'textfield',
		'#title' => t('Hostname or IP address:'),
		'#maxlength' => 64,
		'#size' => 40,
		'#required' => TRUE,
		'#default_value' => '',
		'#description' => t(
			'for which will be enforced following ssl context options ' .
			'verify_peer=false,verify_peer_name=false,allow_self_signed=true'
		),
	);
	$form['actions'] = array(
		'#type' => 'actions'
	);
	$form['actions']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);
	return $form;
}

function trustcert_admin_form_validate($form, &$form_state) {
	$tc_host = $form_state['values']['tc_host'];

	if(!filter_var($tc_host,FILTER_VALIDATE_IP)) {
		if(!preg_match('/^(?![0-9\_]+)([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9]+$/',$tc_host)) {
			form_set_error('host',
				t('Hostname or IP address %host has bad format.',array('%host' => $tc_host)));
		}
	}
	$tc_host = db_select('trustcert','t')
		->fields('t',array('host'))
		->condition('host',$tc_host)
		->execute()
		->fetchField();
	if($tc_host) {
		form_set_error('host',
			t('Host %host already exists.',array('%host' => $tc_host)));
	}
}

function trustcert_admin_add() {
	return drupal_get_form('trustcert_admin_form');
}

function trustcert_admin_form_submit($form, &$form_state) {
	form_state_values_clean($form_state);
	$tc_host = $form_state['values']['tc_host'];
	if($tc_host) {
		db_insert('trustcert')
			->fields(array('host' => strtolower($tc_host)))
			->execute();
		drupal_set_message(t('The host ' . $tc_host . ' has been saved.'));
	}
	$form_state['redirect'] = TRUSTCERT_ADMIN_URI;
}

function trustcert_admin_delete($form, &$form_state, $tc_id) {
	if(!($tc_id > 0)) {
		drupal_goto(TRUSTCERT_ADMIN_URI);
	}
	$form = array();
	$form['tc_id'] = array(
		'#type' => 'value',
		'#value' => $tc_id
	);
	return confirm_form($form,
		 t('Are you sure you want to delete this item?'),
		 TRUSTCERT_ADMIN_URI
	);
}

function trustcert_admin_delete_submit($form, &$form_state) {
	$tc_id = $form_state['values']['tc_id'];
	if(!($tc_id > 0)) {
		drupal_goto(TRUSTCERT_ADMIN_URI);
	}
	if($form_state['values']['confirm']) {
		db_delete('trustcert')
			->condition('id',$tc_id)
			->execute();
		drupal_set_message('You have successfully deleted host.');
	}
	$form_state['redirect'] = TRUSTCERT_ADMIN_URI;
}
