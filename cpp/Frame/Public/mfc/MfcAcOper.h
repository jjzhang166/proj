#pragma once
/*
@author zhp
@date 2017/7/12 10:52
@purpose for mfc account operate
*/
#include <afxwin.h>
#include <boost/shared_ptr.hpp>
#include <AppMix/ConsoleComponent.h>

class MfcAcOperBase : public CDialog{
public:
	virtual ~MfcAcOperBase();
	const std::string& GetAcItemKey() const;
	bool Init(UINT id_template, const std::string& ac_item_key, const stAcSerInfo& ac_info);
	bool RunScript(const std::string& script_name);
	void UpdateUi();
	void ModifyAcItemData(const std::string& item_key, const std::string& key, const std::string& value);
	virtual void OnUpdateUi(const luabind::object& item_data);
	bool Show(bool is_show);
	//发包之类的操作，最好在io线程中。
	ConsoleSessionS* GetSession();
	ConsoleSessionOper* GetSessionOper();

protected:
	stAcSerInfo			ac_info_;
	std::string			ac_item_key_;
};
typedef boost::shared_ptr<MfcAcOperBase> MfcAcOperBasePtr;

class MfcAcOperMgr : public AcOperSink{
	typedef std::map<std::string, MfcAcOperBasePtr> AcOperCont;

public:
	MfcAcOperMgr();
	virtual ~MfcAcOperMgr();
	bool Add(UINT id_template, const stAcSerInfo& ac_info);
	MfcAcOperBase* Find(const std::string& ac_item_key) const;
	MfcAcOperBase* GetFrist() const;

protected:
	virtual MfcAcOperBase* MakeAcOper();
	virtual UINT GetAcOperDlgTemplateId() const = 0;
	void OnAddAc(const stAcSerInfo& ac_info) override;
	void OnDeleteAc(const std::string& item_key) override;
	void OnAcConnected(ConsoleSessionS* session, const std::string& item_key) override;
	void OnAcDisconnect(const std::string& item_key) override;
	void OnUpdateAcItemData(int ac_item_idx, const std::string& item_key, const luabind::object& item_data) override;

protected:
	AcOperCont			ac_opers_;
};
