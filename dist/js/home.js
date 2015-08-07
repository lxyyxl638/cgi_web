var Base_url = "http://localhost/";
var current_dialog;
var current_chat_uid;
$(function(){

	$("#table").delegate("li","click",function(){
		  var friend_uid = $(this).attr("id");
		  
		  current_chat_uid = friend_uid;

		  var tmp = "*[name=" + friend_uid + "]";
		  if ($(tmp).length > 0) {
		  	//已经存在的对话框
		  	current_dialog.hide();
		  	$(tmp).show();
		  	current_dialog = $(tmp);
		  } else {
		  	//另外开启一个
		  	var newDialog = "<div name=" + friend_uid + ">" + "<div>\
            <div name=\"share\">\
            </div>\
            <span name=\"msgs\">Messages:" + friend_uid + "</span>\
            <div name=\"recv_chat_window\" class=\"chat_window\">\
              <table name=\"chat\">\
              </table>\
            </div>\
          </div>\
          <div>\
              <div style=\"float: right; text-align: left\">\
                <span name=\"errors\">\
                </span>\
              </div>\
              <div style=\"float: left; text-align: right\">\
                  <a onclick=\"EmoticonAdv.showDropDown('btnlol', 'text')\" name=\"btnlol\" class=\"emoticon\"></a>\
                  <div name='emoticonDropDown' style='display: none;'>\
                      <ul name=\"connies\">\
                      </ul>\
                  </div>\
                  <span name=\"nickname\"></span>\
                  <button name=\"post\" class=\"send\" onclick=\"send()\">Post</button>\
              </div>\
          </div>\
          <div class=\"row\">\
              <textarea class=\"text\" name=\"content\" style=\"width:100%\">\
              </textarea>\
          </div></div>";
		  	$("#chat_window").append(newDialog);
		  	if ($(current_dialog).length > 0) current_dialog.hide();
		  	$(tmp).show();
		  	current_dialog = $(tmp);
		  }
	});

	$("#active").click(function(){
		
		var friend = new Array();
		for (var i = 0;i < 5;++i) {
			friend[i] = new Array();
			friend[i]['uid'] = i;
			friend[i]['name'] = "朋友" + i;
		}

		for (var i = 0;i < 5;++i) {
			$("#table").append("<li class=\"list-group-item\" id="+ friend[i]['uid'] + ">" + friend[i]['name'] + "</li>");
		}
	});
	

	$("#nav_team").click(function(){
		$("#latest_chat").hide();
		$("#search_friend").hide();
		$.get(Base_url + "get_all_friend_team",function(data,status){
			if (data["result"] == "success" && data["team_list"]) {
				$("#myteam").empty();
				for (x in data["team_list"]) {
					str = "<div class=\"panel panel-default\">\
          						<div class=\"panel-heading\">\
            						<h4 class=\"panel-title\">\
              							<a data-toggle=\"collapse\" data-parent=\"#myteam\" data_id=" + data["team_list"][x]["team_id"] + " data_name = " + data["team_list"][x]["team_name"] + " href=#" + data["team_list"][x]["team_id"] + data["team_list"][x]["team_name"]+">" + data["team_list"][x]["team_name"] + "</a>\
            						</h4>\
          						</div>\
          					<div id=" + data["team_list"][x]["team_id"] + data["team_list"][x]["team_name"] + " class=\"panel-collapse collapse\">\
          					              <div class=\"panel-body\">\
          					              	<ul class=\"list-group\">\
          					              	</ul>\
          					              </div>\
          					</div>";
					$("#myteam").append(str);
				}
			} else if (data["detail"]== "unlogin") {
			     location.href=Base_url + "sign_in";
			}
		});
		
		$("#myteam").show();
	});
	

	$("#myteam").delegate("a","click",function() {
		obj = $(this);

		$.post(Base_url+"get_friends_by_team",{
			team_id : obj.attr("data_id")
		},function(data){
			if (data["result"] == "success") {
				tmp = obj.attr("data_id") + obj.attr("data_name");
				$("#" + tmp + " .panel-body .list-group").empty();

				for (x in data["friend_list"]) {
					if (data["friend_list"][x]["online"] == "1") {
						str =  "<button class=\"list-group-item list-group-item-success\" friend_uid=" + data["friend_list"][x]["friend_id"] + " >\
                        		<span class=\"badge\">14</span>" + data["friend_list"][x]["friend_nickname"] + "</button>";
                        $("#" + tmp + " .panel-body .list-group").append(str);             
                    }
                 }

                 for (x in data["friend_list"]) {
					if (data["friend_list"][x]["online"] == "0") {
						str =  "<button class=\"list-group-item\" friend_uid=" + data["friend_list"][x]["friend_id"] + " >\
                        		<span class=\"badge\">14</span>" + data["friend_list"][x]["friend_nickname"] + "</button>";
                        $("#" + tmp + " .panel-body .list-group").append(str);             
                    }
                 }

			} else if (data["detail"] == "unlogin"){
				location.href=Base_url + "sign_in";
			}
		})
	});
	
	$("[type=search_button]").on("click",function(){
		$.post(Base_url + "query_users",{
			username : $("#search_box").val()
		},function(data){
			if (data["result"] == "success") {
				$("#latest_chat").hide();
				$("#myteam").hide();
				$("#search_friend .list-group").empty();

				for (x in data["user_list"]) {
					if (data["user_list"][x]["is_friend"]) {
						tmp = "<button type=\"button\" class=\"list-group-item\" search_username=" + data["user_list"][x]["username"] +" search_id=" + data["user_list"][x]["user_id"] +  " >" + data["user_list"][x]["nickname"] + "    (点击聊天)</button>";
					} else {
						tmp = "<button type=\"button\" class=\"list-group-item\" search_username=" + data["user_list"][x]["username"] +" search_id=" + data["user_list"][x]["user_id"] +  " data-toggle=\"modal\" data-target=\"#gridSystemModal\" opearation=\"friend_requst\" >" + data["user_list"][x]["nickname"] + "    (添加好友)</button>";
					}
					$("#search_friend .list-group").append(tmp);
				}
				$("#search_friend").show();
			}
		})
	});


	$("[operation=friend_requst]").on("click",function(){
		$("#modal_friend_username").text($(this).attr("search_username"));
		$("#modal_friend_nickname").text($(this).text());
		$("[request-uid]").attr("request-uid") = $(this).attr("search_id");
	})

	$("[button-operation=add_friend]").on("click",function() {

		$.post(Base_url + "send_friend_request", {
			request_uid : $("[request-uid]").attr("request-uid"),
			message : $("[request-uid]").text()
		},function(data) {
			if (data["result"] == "success") {
				alert("已发送请求");
			} else {
				alert("您已经发送过请求");
			}
		})
	});

});




