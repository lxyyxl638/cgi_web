var Base_url = "http://localhost/";
$(function(){

	$("#login").click(function(){
		var message = '';
		var username = $("#username").val();
		var password = $("#password").val();
		
		if  (username == "" || (!isValidUsername(username))) {

			message = "请输入6——16位仅包含字母，数字和下划线的用户名";

		} else if (password == "" || (!isValidPassword(password))) {

			message = "请输入6——16位密码";
		} else {
				$.post("/sign_in",{
                		'username':$("#username").val(),
                		'password':$("#password").val()
              			},function(data){
              			if ($.trim(data['result'])=="fail")
              			{
					message = $.trim(data["detail"]);
					$("#result").html(message);
              			}
                		else
                		{
                			location.href = "/home";
                		}
          		})
		}

                if (message.length > 0) $("#result").html(message);
	});

	$("#signup").click(function(){
		var message = '';
		var username = $("#username_signup").val();
		var password = $("#password_signup").val();
		var nickname = $("#nickname_signup").val();
		var sex = $("#sex").val();

		if  (username == "" || (!isValidUsername(username))) {

			message = "请输入6——16位仅包含字母，数字和下划线的用户名";

		} else if (password == "" || (!isValidPassword(password))) {

			message = "请输入6——16位密码";
		} else if (nickname == "" || (!isValidNickname(nickname))) {

			message = "请输入少于10个字符的昵称";
		} else if (sex == "") {

			message = "请选择您的性别";
		} else {
					$.post("/sign_up",{
						'type' : 2,
						'username':username,
						'password':password,
						'nickname':nickname,
						'sex':sex
					},function(data) {
						if ($.trim(data['result'])=='fail') {

							message = $.trim(data['detail']);
							$("#result").html(message);
						} else {
							location.href = "/sign_in";
						}
			   		})
				}
		if (message.length > 0) $("#result").html(message);
	});

	$("#username_signup").blur(function(){

		var username = $("#username_signup").val();
		var message = "";
		if  (username == "" || (!isValidUsername(username))) {
			message = "请输入6——16位仅包含字母，数字和下划线的用户名";
		} else {
				$.post("/sign_up",{
				'type' : 1,
				'username':username
				},function(data) {
					if ($.trim(data['result'])=='fail') {

						message = $.trim(data['detail']);
						$("#username_result").html(message);
							
					} else {
						message="可以使用的用户名";
						$("#username_result").html(message);
					}
			   	})
			}
		if (message.length > 0) $("#username_result").html(message);
	});
});

function isValidUsername(username) {

	var reg = /^\w+$/;

	if (username.length < 6 || username.length >16 || reg.test(username) == false) {
		return false;
	} else {
		return true;
	}
}

function isValidPassword(password) {

	var reg = /^(\w){6,16}$/;

	if (password.length < 6 || password.length >16 || reg.test(password) == false) {
		return false;
	} else {
		return true;
	}
}

function isValidNickname(nickname) {

	if  (nickname.length >10) {
		return false;
	} else {
		return true;
	}
}
