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
	// 	var username = $("#username").val();
	// 	var password = $("#password").val();
		
	// 	if  (username == "" || (!isValidUsername(username))) {

	// 		message = "请输入6——16位仅包含字母，数字和下划线的用户名";

	// 	} else if (password == "" || (!isValidPassword(password))) {

	// 		message = "请输入6——16位密码";
	// 	} else {
	// 			$.post(Base_url + "sign_in",{
 //                		'username':$("#username").val(),
 //                		'password':$("#password").val()
 //              			},function(data){
 //              			if ($.trim(data['result'])=="fail")
 //              			{
	// 				message = $.trim(data["detail"]);
	// 				$("#result").html(message);
 //              			}
 //                		else
 //                		{
 //                			location.href = Base_url + "home";
 //                		}
 //          		})
	// 	}

 //                if (message.length > 0) $("#result").html(message);
	// });

});

