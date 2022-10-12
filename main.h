#pragma once

#include <BML/BMLAll.h>

extern "C" {
	__declspec(dllexport) IMod* BMLEntry(IBML* bml);
}

class FirstPersonCamera : public IMod {
public:
	FirstPersonCamera(IBML* bml) : IMod(bml) {}

	virtual CKSTRING GetID() override { return "FirstPersonCamera"; }
	virtual CKSTRING GetVersion() override { return "0.0.1"; }
	virtual CKSTRING GetName() override { return "First Person Camera"; }
	virtual CKSTRING GetAuthor() override { return "BallanceBug"; }
	virtual CKSTRING GetDescription() override { return "Let you see the what your ball actually sees."; }
	DECLARE_BML_VERSION;

	void OnLoad() override;
	void OnModifyConfig(CKSTRING category, CKSTRING key, IProperty* prop) override;
	void OnProcess() override;
	void OnStartLevel() override;
	void OnUnpauseLevel() override;
	void OnPostStartMenu() override;

private:
	void enter_fp_cam();
	void exit_fp_cam();

	IProperty *prop_enabled = nullptr, *prop_speed = nullptr;
	bool enabled = false, is_in = false;
	float adjust_speed = 0.012f;
	CKCamera *fp_cam = nullptr;
	CKDataArray *current_level_ = nullptr;
	bool init_ = false;
};
