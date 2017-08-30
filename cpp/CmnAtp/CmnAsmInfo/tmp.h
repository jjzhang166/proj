
struct stAtp_FastLogin{
	void*	ukn_ptr_;			//$34，增加登陆项的时候会变化
	int		qq_cnt_;			//$78
	int		cur_selected_;
	int		ukn_dw_;
	int		ukn_dw1_;
	void*	ukn_ptr1_;			//$88
};
//sizeof(stAtp_FastLogin) == 0xb8


struct stQqString{
	char	str_[0x80];
};

struct stQqPair{
	stQqString		qq_;
	stQqString		pwd_;
};

struct stQqPairMgr{
	int			qq_cnt_;
	stQqPair	qqs_[2];
};