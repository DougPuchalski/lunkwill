window.lw_login_form = ->
	login_form=document.createElement("form")
	login_form.action="
		javascript:window.ajax((document.getElementById('email').value+
		'@##@'+document.getElementById('password').value)),window.ajax('')"

	login_form.innerHTML="
		<strong>Email:</strong><br><input id=email type=email></input><p>
		<strong>Password:</strong><br><input id=password type=password></input><p>
		<input type=submit value=Login></input>"

	window.EvalScriptElem.parentNode.insertBefore login_form, window.EvalScriptElem

window.lw_logout_button = ->

	logout=document.createElement("a")

	logout.setAttribute 'onclick', 
		"javascript:window.setCookie('login', ''),window.ajax('')"

	logout.setAttribute 'style', 'background:#aa2211;color:#FFF;
		margin-right:5px;width:9%;vertical-align: top;'

	logout.innerHTML = "<div style='margin:1px 10px;display: inline-block;'>
		Logout</div>"

	window.EvalScriptElem.parentNode.insertBefore logout, window.EvalScriptElem
	
