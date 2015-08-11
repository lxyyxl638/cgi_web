var Base_url = "http://localhost/";
var current_dialog;
var current_chat_uid;
$(function(){

	$("#nav_latest_chat").on("click",function(){
		$("#nav_latest_chat a span").text('');
		$("#myteam").hide();
		$("#search_friend").hide();
		$("#notification_list").hide();
		$("#latest_chat").show();
	});

	$("#table").delegate("li","click",function(){
		add_chat(this,true);
	});
	

	$("#myteam").delegate("[friend_uid]","click",function() {

		add_latest_chat($(this).attr('friend_uid'),$(this).text(),$(this).hasClass('list-group-item-success'),true);
	})
	// $("#active").click(function(){
		
	// 	var friend = new Array();
	// 	for (var i = 0;i < 5;++i) {
	// 		friend[i] = new Array();
	// 		friend[i]['uid'] = i;
	// 		friend[i]['name'] = "朋友" + i;
	// 	}

	// 	for (var i = 0;i < 5;++i) {
	// 		$("#table").append("<li class=\"list-group-item\" id="+ friend[i]['uid'] + ">" + friend[i]['name'] + "</li>");
	// 	}
	// });
	

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
						str =  "<button class=\"list-group-item list-group-item-success\" friend_uid=" + data["friend_list"][x]["friend_id"] + " friend_username=" + data['friend_list'][x]['friend_name'] + " >\
                        		<span class=\"badge\"></span>" + data["friend_list"][x]["friend_nickname"] + "</button>";
                        $("#" + tmp + " .panel-body .list-group").append(str);             
                    }
                 }

                 for (x in data["friend_list"]) {
					if (data["friend_list"][x]["online"] == "0") {
						str =  "<button class=\"list-group-item\" friend_uid=" + data["friend_list"][x]["friend_id"] +  " friend_username=" + data['friend_list'][x]['friend_name'] + " >\
                        		<span class=\"badge\"></span>" + data["friend_list"][x]["friend_nickname"] + "</button>";
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
				$("#notification_list").hide();
				$("#search_friend .list-group").empty();
				tmp = "<button type=\"button\" class=\"list-group-item disabled\" >快加他们为好友吧~</button>";
				$("#search_friend .list-group").append(tmp);
				for (x in data["user_list"]) {
					if (data["user_list"][x]["is_friend"] == "0") {
						tmp = "<button type=\"button\" class=\"list-group-item\" search_username=" + data["user_list"][x]["username"] +" search_id=" + data["user_list"][x]["user_id"] +  " data-toggle=\"modal\" data-target=\"#gridSystemModal\" operation=\"friend_request\" ><span class='badge'>"+data["user_list"][x]["username"]+"</span>" + data["user_list"][x]["nickname"] + "</button>";
						$("#search_friend .list-group").append(tmp);
					}
				}
				tmp = "<br><button type=\"button\" class=\"list-group-item disabled\" >他们已经是你的好友~</button>";
				$("#search_friend .list-group").append(tmp);
				
				for (x in data["user_list"]) {
					if (data["user_list"][x]["is_friend"] == "1") {
						tmp = "<button type=\"button\" class=\"list-group-item list-group-item-success\" search_username=" + data["user_list"][x]["username"] +" search_id=" + data["user_list"][x]["user_id"] +  " ><span class='badge'>" + data["user_list"][x]["username"] + "</span>" + data["user_list"][x]["nickname"] + "</button>";
						$("#search_friend .list-group").append(tmp);
					} 					
				}				
				

				$("#search_friend").show();
			}
		})
	});



	$("[button-operation=add_friend]").on("click",function() {

		$.post(Base_url + "send_friend_request", {
			request_uid : $("[request-uid]").attr("request-uid"),
			message : $("[request-uid]").val()
		},function(data) {
			if (data["result"] == "success") {
				alert("已发送请求");
			} else {
				alert("您已经发送过请求");
			}
			$('#gridSystemModal').modal('hide')
		})
	});


	$('#gridSystemModal').on('show.bs.modal', function (e) {
	        obj = e.relatedTarget;
		$("#modal_friend_username").text(obj.attributes.search_username.value);
		$("#modal_friend_nickname").text(obj.textContent);
		$("[request-uid]").attr("request-uid",obj.attributes.search_id.value);
	  });

	/*获取通知*/
	$("#nav_personal_center").on("click",function() {
		get_notification();
	});
	

	$('#gridSystemModalConfirm').on('show.bs.modal', function (e) {
	        obj = e.relatedTarget;
	        $("#gridSystemModalConfirm").attr("notification_id",e.relatedTarget.attributes.notification_id.value);
	       	$("#gridSystemModalConfirm").attr("request_uid",e.relatedTarget.attributes.friend_id.value);
	        if (obj.attributes.response!=undefined) {
	        	//是接受请求
	        	$("#handle_request").hide();
	        } else {
	        	$("#handle_request").show();
	        }
	        ask_team();
		// $("#modal_friend_username").text(obj.attributes.search_username.value);
		// $("#modal_friend_nickname").text(obj.textContent);
		// $("[request-uid]").attr("request-uid",obj.attributes.search_id.value);
	  });

	$("#handle_request").change(function() {
		if ($(this).val() == "reject") {
			$("#select_team").hide();
		} else {
			$("#select_team").show();
		}
	});



	$("[button-operation=confirm_friend]").on("click",function() {

		$.post(Base_url + "confirm_friend_request", {
			no_id : $("#gridSystemModalConfirm").attr("notification_id"),
			request_uid : $("#gridSystemModalConfirm").attr("request_uid"),
			message : $("#handle_request").val(),
			team_id : $("#ask_teams").val()
		},function(data) {
			if (data["result"] == "success") {
				alert("已发送请求");
			} else {
				alert("您已经发送过请求");
			}
			$('#gridSystemModalConfirm').modal('hide');
			get_notification();
		})
	});

	$("#button_add_new_team").on("click",function() {
		if ($("#add_new_team").is(":hidden")) {
			$("#add_new_team").show();
		} else {
			if ($("#input_team_name").val().length > 0) {
			$.post(Base_url + "add_new_team",{
				team_name : $("#input_team_name").val()
			},function(data){
				if (data["result"] == "success") {
					alert("添加成功");
					$("#add_new_team").hide();
					ask_team();
				} else {
					alert($data["detail"]);
				}
			})
		   } else {
		   	  alert("请输入组名");
		   }
		}
	})
	// $("#ask_teams").on("click",function(){
	// 	$("#ask_teams").empty();
	// 	$.get(Base_url+"get_all_friend_team",function(data) {
	// 		if (data["result"] == "success" && data["team_list"]) {
	// 			for (x in data["team_list"]) {
	// 				tmp="<option value="+ data["team_list"][x]["team_id"]+" >" + data["team_list"][x]["team_name"] + "</option>";
	// 				$("#ask_teams").append(tmp);
	// 			}
	// 		}
	// 	})
	// });

	$('[response="reject"]').on("click",function(){
		$.post(Base_url + "confirm_friend_request", {
			no_id : $(this).attr("notification_id"),
			request_uid : $(this).attr("friend_id"),
			message : "",
			team_id : "0"
		},function(data) {
				
			get_notification();
		})
	})



	get_latest_chat();
	/*轮询通知数目*/
	setInterval(ask_notification_num,10000);

});

function ask_notification_num() {
	$.get(Base_url+"get_notification_num",function(data) {
			 if (data["result"] == "success") {
			 	if (data["num"] > 0) {
			 		$("#nav_personal_center .badge").text(data["num"]);
			 	} else {
			 		$("#nav_personal_center .badge").text('');

			 	}
			 }
	});
}

function ask_team() {
	$("#ask_teams").empty();
		$.get(Base_url+"get_all_friend_team",function(data) {
			if (data["result"] == "success" && data["team_list"]) {
				for (x in data["team_list"]) {
					tmp="<option value="+ data["team_list"][x]["team_id"]+" >" + data["team_list"][x]["team_name"] + "</option>";
					$("#ask_teams").append(tmp);
				}
			}
		})
}


function get_notification() {
$.get(Base_url+"get_notification",function(data) {
			 if (data["result"] == "success") {

			 	 	$("#latest_chat").hide();
					$("#myteam").hide();
					$("#search_friend").hide();
					$("#notification_list .list-group").empty();
			 	 for (x in data["notification_list"]) {
			 	 	if (data["notification_list"][x]["state"] == "0") {
			 	 		//有人要添加我为好友
			 	 		tmp="<li class=\"list-group-item\" notification_id=" + data["notification_list"][x]["no_id"] + " friend_id=" + data["notification_list"][x]["send_id"] + " data-toggle=\"modal\" data-target=\"#gridSystemModalConfirm\" ><span class=\"badge\" style=\"cursor:pointer\" >处理请求</span>"+ data["notification_list"][x]["send_nickname"] +"想添加你为好友:" +  data["notification_list"][x]["additional_message"] +"</li>"
			 	 	} else if (data["notification_list"][x]["state"] == "1") {
			 	 		//收到了别人的消息
			 	 		if (data["notification_list"][x]["additional_message"] == "accept") {
			 	 			tmp="<li class=\"list-group-item\" notification_id=" + data["notification_list"][x]["no_id"] + " friend_id=" + data["notification_list"][x]["send_id"] + " response=" + data["notification_list"][x]["additional_message"] + " data-toggle=\"modal\" data-target=\"#gridSystemModalConfirm\" ><span class=\"badge\" style=\"cursor:pointer\" >处理请求</span>" + data["notification_list"][x]["send_nickname"] + "接受了你的请求</li>"
			 	 		} else {
			 	 			tmp="<li class=\"list-group-item\" notification_id=" + data["notification_list"][x]["no_id"] + " friend_id=" + data["notification_list"][x]["send_id"] + " response=" + data["notification_list"][x]["additional_message"] + " ><span class=\"badge\" style=\"cursor:pointer\" >知道了</span>" + data["notification_list"][x]["send_nickname"] + "拒绝了你的请求</li>"
			 	 		}
			 	 	}
			 	 	$("#notification_list .list-group").append(tmp);

			 	 }
			 	  $("#notification_list").show();
			 }
		})
}


//获取最近未读消息
function get_latest_chat() {
	$.get(Base_url+"get_latest_chat",function(data){
		if (data['result'] == "success") {
			obj = data["friend_list"];
			for (x in data["friend_list"]) {
				if (obj[x]["is_online"] == "1") {
					$("#table").append("<li style=\"cursor:pointer\" class=\"list-group-item list-group-item-success\" id="+ obj[x]["friend_id"] + " friend_username=" + obj[x]['friend_username'] + " friend_nickname=" +obj[x]['friend_nickname']+ ">" + obj[x]['friend_nickname'] + "(" + obj[x]['friend_username']+ ")" + "<span class=\"badge\">" + obj[x]['num']+"</span></li>");
				}
			}

			for (x in data["friend_list"]) {
				if (obj[x]["is_online"] == "0") {
					$("#table").append("<li style=\"cursor:pointer\" class=\"list-group-item\" id="+ obj[x]["friend_id"] + " friend_username=" + obj[x]['friend_username'] + " friend_nickname=" +obj[x]['friend_nickname']+ ">" + obj[x]['friend_nickname'] + "(" + obj[x]['friend_username']+ ")" + "<span class=\"badge\">" + obj[x]['num']+"</span></li>");
				}
			}

		}
	})
}

function get_unread_message(obj) {
	$.post(Base_url+"get_unread_message",{
		"request_uid" : $(obj).attr("id")
	},function(data){
		if (data["result"] == "success") {
			for (x in data["message_list"]) {
				addmsg($(obj).attr("friend_id"),$(obj).attr("friend_nickname"),data['message_list'][x]['message'],true);
			}
		}
	})
}

function add_chat(obj,is_show) {
	var friend_username = $(obj).attr("friend_username");
	var friend_name = $(obj).attr("friend_nickname");
	var friend_uid = $(obj).attr("id");
		$("[id=" + friend_uid + "] span").text('');

		  current_chat_uid = friend_uid;

		  var tmp = "*[name=" + friend_uid + "]";
		  if ($(tmp).length > 0) {
		  	//已经存在的对话框
			if (is_show) {
		  		if (current_dialog != undefined)current_dialog.hide();
		  		$(tmp).show();
		  		current_dialog = $(tmp);
			}
		  } else {
		  	//另外开启一个
		  	var newDialog = "<div name=" + friend_uid + ">" + "<div>\
            <div name=\"share\">\
            </div>\
            <span name=\"msgs\">正在和" + friend_name + "(" + friend_username +")" + "对话</span>\
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
                  <button name=\"post\" class=\"send\" onclick=\"send()\" post_uid=" + friend_uid + " >发送</button>\
              </div>\
          </div>\
          <div class=\"row\">\
              <textarea class=\"text\" name=\"content\" style=\"width:100%\">\
              </textarea>\
          </div></div>";
		  	$("#chat_window").append(newDialog);
		  	if (is_show) {
		  		if ($(current_dialog).length > 0) current_dialog.hide();
		  		$(tmp).show();
		  		current_dialog = $(tmp);
		  		get_unread_message(this);
		  	}
		  }
}

function add_latest_chat(friend_uid,friend_nickname,is_online,is_show) {
	
	//先删除自己，然后再置顶
	if ($("#latest_chat [id=" + friend_uid +"]").length > 0) {
		$("#latest_chat [id=" + friend_uid +"]").remove();
	}
	$("#table").prepend("<li style=\"cursor:pointer\" class=\"list-group-item\" id="+ friend_uid + " friend_username=" + friend_nickname + " friend_nickname=" + friend_nickname+ ">" + friend_nickname + "(" + friend_nickname + ")" + "<span class=\"badge\">" + "</span></li>");
	if (is_online) {
		$("#latest_chat [id='" + friend_uid + "']").addClass('list-group-item-success');
	}

	obj =  "#latest_chat [id='" + friend_uid + "']"
	add_chat(obj,is_show);
}

function add_unread_badge(friend_uid) {
	$("#latest_chat [id=" + friend_uid +"] span").text("未读消息");
}

function logout() {
	$.get(Base_url + "logout",function() {
		location.href = Base_url + "sign_in";
	})
}

