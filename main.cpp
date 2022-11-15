#include "main.h"

IMod* BMLEntry(IBML* bml) {
	return new FirstPersonCamera(bml);
}

void FirstPersonCamera::OnLoad() {
	GetConfig()->SetCategoryComment("Main", "Main settings.");
	prop_enabled = GetConfig()->GetProperty("Main", "Enabled");
	prop_enabled->SetDefaultBoolean(true);
	prop_enabled->SetComment("I think you already know what this does.");
	enabled = prop_enabled->GetBoolean();
	prop_speed = GetConfig()->GetProperty("Main", "CameraAdjustSpeed");
	prop_speed->SetDefaultFloat(0.012f);
	prop_speed->SetComment("Determines how quick your camera will adjust to the movement of your balls. Default: 0.012");
	adjust_speed = prop_speed->GetFloat();
	fp_cam = static_cast<CKCamera*>(m_bml->GetCKContext()->CreateObject(CKCID_CAMERA, "FirstPersonCam"));
}

void FirstPersonCamera::OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop) {
	enabled = prop_enabled->GetBoolean();
	adjust_speed = prop_speed->GetFloat();
}

void FirstPersonCamera::OnProcess() {
	if (!enabled || !m_bml->IsPlaying() || !is_in)
		return;

	auto player_ball = static_cast<CK3dObject*>(current_level_->GetElementObject(0, 1));
	VxVector camera_pos, ball_pos;
	fp_cam->GetPosition(&camera_pos);
	player_ball->GetPosition(&ball_pos);
	if (SquareMagnitude(camera_pos - ball_pos) > 0.04f) {
		fp_cam->SetPosition(Interpolate(adjust_speed * m_bml->GetTimeManager()->GetLastDeltaTime(), camera_pos, ball_pos));
		fp_cam->LookAt(ball_pos);
	}

	if (m_bml->GetInputManager()->IsKeyDown(CKKEY_LSHIFT)) {
		CKCamera* cam = m_bml->GetTargetCameraByName("InGameCam");
		fp_cam->SetWorldMatrix(cam->GetWorldMatrix());
	}
}

void FirstPersonCamera::OnStartLevel() {
	if (!enabled) {
		if (is_in)
			exit_fp_cam();
		return;
	}
	enter_fp_cam();
}

void FirstPersonCamera::OnUnpauseLevel() {
	if (!enabled) {
		if (is_in)
			exit_fp_cam();
		return;
	}
	if (!is_in)
		enter_fp_cam();
}

void FirstPersonCamera::OnPostStartMenu() {
	if (init_)
		return;
	current_level_ = m_bml->GetArrayByName("CurrentLevel");
	init_ = true;
}

void FirstPersonCamera::enter_fp_cam() {
	m_bml->AddTimer(10u, [this] {
		CKCamera *cam = m_bml->GetTargetCameraByName("InGameCam");
		fp_cam->SetWorldMatrix(cam->GetWorldMatrix());
		int width, height;
		cam->GetAspectRatio(width, height);
		fp_cam->SetAspectRatio(width, height);
		fp_cam->SetFov(cam->GetFov());
		m_bml->GetRenderContext()->AttachViewpointToCamera(fp_cam);
		is_in = true;
	});
}

void FirstPersonCamera::exit_fp_cam() {
	CKCamera* cam = m_bml->GetTargetCameraByName("InGameCam");
	m_bml->GetRenderContext()->AttachViewpointToCamera(cam);
	is_in = false;
}
