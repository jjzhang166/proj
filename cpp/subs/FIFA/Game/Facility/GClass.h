#pragma once
/*
@author zhp
@date 2014-2-18 13:26
@purpose for game class
*/
#include "GTypes.h"
#include <string>
#include "Common/GObjProxy.h"
#include "Protocol/P_Defines.h"
#include <queue>
#include "json/value.h"
#include "Common/UsefulClass.h"
#include "Net/PocoMore.h"

//#define D_SOME_LOG

namespace GClass
{
	//处理http包
	class HandleHttpPackage{
		HandleHttpPackage();
		~HandleHttpPackage();
		//处理http字符
	public:
		static HandleHttpPackage* GetInstance();
		//更新
		static void UpdateHandler( const char* http_package );
		//post string
		const char* Post();
		//get string
		const char* Get();
		//http version
		const char* HttpVersion();
		//host
		const char* Host( const char* http_package = nullptr );
		//accept
		const char* Accept();
		//cookie
		const char* Cookie( const char* http_package = nullptr );
		//connection
		const char* Connection();
		//content type
		const char* ContentType();
		//content gzip type
		const char* ContentGzipType();
		//设置host
		void SetHost( const std::string& str_host );
		const std::string& GetHost() const;
		//设置cookie
		void SetCookie( const Poco::Net::NameValueCollection& cookie );
		Poco::Net::NameValueCollection& GetCookie();

	public:
		//在一个http包中，索引到method
		FindBetweenStringA::RangePtr GetMethodFromHttpPackage( const char* http_package ) const;
		//在一个http包中，索引到uri
		FindBetweenStringA::RangePtr GetUriFromHttpPackage( const char* http_package ) const;

	public:
		//得到http包的msg的第一个字符的地址
		static const char* GetFirstHttpMsgCharAddr( const char* str_http );
		//content type是否是json格式的
		static bool ContentTypeIsJson( const char* str_http );
		//content type是否是x-gzip
		static bool ContentTypeIsXgzip( const char* str_http );

	private:
		static HandleHttpPackage*			handler_;
		std::string							host_;
		Poco::Net::NameValueCollection		cookie_;
	};
	//发包http字符串处理
	class HandleSendPackage{
		HandleSendPackage();
		//静态函数
	public:
		static HandleSendPackage& GetMe();

		//组包相关函数
	public:
		//encode content length
		const char* EncodeContentLength( const char* msg );
		const char* EncodeContentLength( std::size_t msg_size );
		//组合http package头，以后考虑使用HTTPRequest_RFB类，现在还未出现问题，暂且将就着
		std::string HttpPackage_CombineNormalMsgHead( const char* msg_catalog, const char* msg );
		std::string HttpPackage_CombineNormalMsgHead( const char* msg_catalog, const std::string& msg );
		//组合经过gzip的包头
		std::string HttpPackage_CombineGzipMsgHead( const char* msg_catalog, const std::string& msg );
		//组合post消息
		std::string HttpPackage_CombinePostMsgHead( const char* uri );
		
		//分析http package
	public:
		//处理游戏中的发包
		void HandleSendPakcageOfGame( const std::string& package_send );

	private:
		//组合消息头
		std::string HttpPackage_CombineNormalMsgHead_( const char* msg_catalog );
	};

	//send http package
	class HttpPackageSend{
	private:
		HttpPackageSend();

	public:
		static HttpPackageSend& GetMe();
		//send implemental
		GType::enHttpSendCall_Result SendHttpPackage( const char* msg_head, const char* msg, std::size_t msg_size, bool is_async );
		GType::enHttpSendCall_Result SendHttpPackage( const char* msg_head, const std::string& msg, bool is_async );
		//异步
		//static GType::enHttpSendCall_Result SendHttpPackageAsync( const char* msg_head, const char* msg );
		static GType::enHttpSendCall_Result SendHttpPackageAsync( const char* msg_head, const std::string& msg );
		//同步
		//static GType::enHttpSendCall_Result SendHttpPackageSync( const char* msg_head, const char* msg );
		//发送通用消息，异步
		//static GType::enHttpSendCall_Result SendNormalMsgAsync( const char* uri, const char* msg );
		static GType::enHttpSendCall_Result SendNormalMsgAsync( const char* uri, const std::string& msg );
		//发送post消息
		static bool SendGetMsgAsync( const char* uri );
		//发送通用消息，同步
		//static GType::enHttpSendCall_Result SendNormalMsgSync( const char* uri, const char* msg );
		//发送json消息，异步
		static GType::enHttpSendCall_Result SendJsonMsgAsync( const char* uri, const Json::Value& msg );
		//发送加密消息，异步
		static GType::enHttpSendCall_Result SendEncryptedJsonMsgAsync( const char* uri, const Json::Value& msg );
		//开始执行发包
		void BeginSendHttp();
		//结束收包动作
		void EndRecvHttp();
		//重置发包计数
		void ResetSendCount();
		//发包碰撞
		void SendPackageHit();

		//请求和响应对
	public:
		//发送json消息，异步
		static bool Post_JsonMsgAsync( const char* uri, const Json::Value* msg );
		static bool Post_JsonMsgSync( const char* uri, const Json::Value* msg, Json::Value& respond_msg );
		//get方法
		static bool Get_JsonMsgAsync( const char* uri );
		static bool Get_JsonMsgSync( const char* uri, Json::Value& respond_msg );
		//加密的消息
		static bool Post_EncryptJsonMsgAsync( const char* uri, const Json::Value& msg );
		static bool Post_EncryptJsonMsgSync( const char* uri, const Json::Value& msg, Json::Value& respond_msg );

		//修改发包相关
	public:
		//修改发包消息
		static void ReplaceMsg( const char* msg_head, const std::string& msg, GType::stHttpSendCall_Content* msg_src );
		static void ReplaceMsg_Uri( const char* uri, const std::string& msg, GType::stHttpSendCall_Content& msg_src );
		static void ReplaceMsg_Json( const char* uri, const Json::Value& msg, GType::stHttpSendCall_Content& msg_src );

	private:
		static GType::enHttpSendCall_Result SendHttpPackage_Impl( GType::stHttpSendCall_Content* http_content, GType::HttpSendCall_NeededObj2* obj2, 
			GType::HttpSendCall_NeededObj1* obj1, int msg_size );
		static GType::enHttpSendCall_Result DoSend_Impl( const char* http_content, std::size_t content_size );
		static void SendRequestImpl( const std::string& http_package, const std::shared_ptr<HTTPRequest_RFB>& request );
		static void SendRequestImplSync( const std::string& http_package, std::shared_ptr<HTTPResponse_RFB>& respond );

	private:
		typedef std::pair<int, int> SendCountT;

	private:
		//调整发包计数
		void RectifySendingCount();

	private:
		int					game_sending_;				//游戏正在发包
		SendCountT			rectify_send_count_;		//调整发包辅助
		DateTimeLocal		last_rec_time_;
		bool				reset_sending_cnt_;
		SharedMutex			reset_sending_mutex_;
	};

	//处理收包
	class HandleRecvPackage{
		HandleRecvPackage();

	public:
		//用于自己发包，然后处理收包
		struct stHttpQuestInfo{
			std::string				method_;			//请求方法
			std::string				uri_;				//http uri

		public:
			const std::string& GetMethod() const;
			void SetMethod( const std::string& method );
			const std::string& GetUri() const;
			void SetUri( const std::string& uri );
		};

	public:
		static HandleRecvPackage& GetMe();
		//////////////////////////////////////////////////////////////////////////
		//由游戏线程调用
		//接收收包
		void RecvHttpPackage( const char* str_http, int package_length );
		//处理后续的包，由游戏线程调用
		void HandleFollowPackage( const char* str_follow, size_t package_length );
		//////////////////////////////////////////////////////////////////////////
		//由注入线程执行
		//http package parse
		void RecvHttpPackageParse( const std::string& _str_http_ );
		//follow package parse
		void FollowPackageParse( const std::string& _str_follow );
		//处理完整的包
		void HandleFullRecvPackage( const std::string& msg_head, const std::string& msg );
		//得到content-length
		static int GetContentLength( const char* str_http );
		//得到http msg length，带http开头的包
		static size_t GetHttpMsgLength( const char* str_http, size_t package_length );
		//http包头的长度，从http到msg之间的长度
		static int GetHttpHeaderLength(const char* str_http, int package_length);
		//得到后续的某个包的msg长度
		static size_t GetFollowMsgLength( const char* str_follow, size_t package_length );

	private:
		//得到第一个tag(无效字符)的地址。这个函数是用于解析ssl_read底层的吧，现已废弃，2016/8/25 23:05
		static const char* GetFirstTagCharAddr( const char* str_package, size_t package_length );

		//自己发的包，自己接收
	public:
		//执行一个收包，由游戏主线程调用
		void DoSllRead( const std::shared_ptr<stHttpQuestInfo>& quest );
		void DoRequest_SllRead( const std::shared_ptr<HTTPRequest_RFB>& quest );
		void DoRequest_SllReadSync( std::shared_ptr<HTTPResponse_RFB>& respond );
		//收到一个包，由注入线程调用
		void OnRecvHttpResult( const std::shared_ptr<stHttpQuestInfo>& quest, const HTTPResponse_RFB& respond );
		void OnRequestHttpResult( const HTTPRequest_RFB& quest, const HTTPResponse_RFB& respond );
		//收到一个json包
		void OnRecvJsonResult( const std::shared_ptr<stHttpQuestInfo>& quest, const Json::Value& result_msg );
		void OnRequestJsonResult( const HTTPRequest_RFB& quest, const Json::Value& result_msg );

	private:
		//是否指定数目的0标记
		static bool IsSpecifyZeroTag( const char* str_package, size_t package_length );

		//type definition
	private:
		struct Sequent{
			std::string				str_anterior_;
			int						left_package_len_;
		};

	private:
		SharedMutex				mutex_handle_follow_;		//用于处理后续
		std::queue<Sequent>		sequents_;					//有后续的包
		char					ssl_read_buffer[4000];
	};
}