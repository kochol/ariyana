using System;
using ari.net;
using System.Collections;

namespace ari.biz
{
	class GoogleAnalytics
	{
		String			m_sTrackingID = null ~ delete _,
						m_sAppName = null ~ delete _,
						m_sAppVer = null ~ delete _,
						m_sPlayerID = null ~ delete _;
		HttpClientService m_http;

		public this(String tracking_id,
			String app_name,
			String app_version,
			String player_id,
			HttpClientService http)
		{
			m_sTrackingID = new String(tracking_id);
			m_sAppName = new String(app_name);
			m_sAppVer = new String(app_version);
			if (player_id != null)
				m_sPlayerID = new String(player_id);
			m_http = http;
			m_http.session.SetVerb(.Post);
		}

		void SendToGA(String hitType, String OptionalParams/*, String PayLoad*/)
		{
			HttpRequest req = .();
			/*var h = new List<String>(1);
			var s = new String("Content-Type: ");
			s.Append(PayLoad == null ? "text/plain": "application/x-www-form-urlencoded; charset=utf-8");
			h.Add(s);
			req.SetHeaders(h);*/
			req.Url = new String();
			req.Url.Append("http://www.google-analytics.com/collect?v=1&t=",
				hitType, "&tid=", m_sTrackingID);
			if (!String.IsNullOrEmpty(m_sPlayerID))
				req.Url.Append("&cid=", m_sPlayerID);
			req.Url.Append("&an=", m_sAppName, "&av=", m_sAppVer,
				"&ds=app&", OptionalParams);

			//HttpRequest->SetContentAsString(Payload);

			m_http.AddRequest(ref req);
		}

		//------------------------------------------------------------------------------------
		// Track the screen view
		//------------------------------------------------------------------------------------
		public void TrackScreenView(String screenName)
		{
			String strParams = scope String("cd=");
			strParams.Append(screenName);
			SendToGA("screenview", strParams);

		} // TrackScreenView

		//------------------------------------------------------------------------------------
		// Ends the player session
		//------------------------------------------------------------------------------------
		public void EndSession()
		{
			String strParams = scope String("cd=end&sc=end");
			SendToGA("screenview", strParams);

		} // Ends the player session

		//------------------------------------------------------------------------------------
		// Sends the event to GA
		//------------------------------------------------------------------------------------
		public void Event(String Category, String Action, String Label, int32 Value)
		{
			String strParams = scope String("ec=");
			strParams.Append(Category, "&ea=", Action, "&el=", Label, "&ev=");
			strParams.AppendF("%d", Value);
			SendToGA("event", strParams);
		}
		public void Event(String Category, String Action, String Label)
		{
			String strParams = scope String("ec=");
			strParams.Append(Category, "&ea=", Action, "&el=", Label);
			SendToGA("event", strParams);
		}
		public void Event(String Category, String Action, int32 Value)
		{
			String strParams = scope String("ec=");
			strParams.Append(Category, "&ea=", Action, "&ev=");
			strParams.AppendF("%d", Value);
			SendToGA("event", strParams);
		}
		public void Event(String Category, String Action)
		{
			String strParams = scope String("ec=");
			strParams.Append(Category, "&ea=", Action);
			SendToGA("event", strParams);
		} // Event

		//------------------------------------------------------------------------------------
		// Sends the custom timing to GA
		//------------------------------------------------------------------------------------
		public void Timing(String Category, String Action, String Label, int32 TimeMiliSec)
		{
			String strParams = scope String("utc=");
			strParams.Append(Category, "&utv=", Action, "&utl=", Label, "&utt=");
			strParams.AppendF("{}", TimeMiliSec);
			SendToGA("timing", strParams);
		} 
		public void Timing(String Category, String Action, int32 TimeMiliSec)
		{
			String strParams = scope String("utc=");
			strParams.Append(Category, "&utv=", Action, "&utt=");
			strParams.AppendF("{}", TimeMiliSec);
			SendToGA("timing", strParams);
		} // Timing

	}
}
