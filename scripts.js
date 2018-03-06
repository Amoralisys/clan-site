function getCookie(name) {
  var matches = document.cookie.match(new RegExp(
    "(?:^|; )" + name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1') + "=([^;]*)"
  ));
  return matches ? decodeURIComponent(matches[1]) : undefined;
}
function setCookie(name, value, options) {
  options = options || {};

  var expires = options.expires;

  if (typeof expires == "number" && expires) {
    var d = new Date();
    d.setTime(d.getTime() + expires * 1000);
    expires = options.expires = d;
  }
  if (expires && expires.toUTCString) {
    options.expires = expires.toUTCString();
  }

  value = encodeURIComponent(value);

  var updatedCookie = name + "=" + value;

  for (var propName in options) {
    updatedCookie += "; " + propName;
    var propValue = options[propName];
    if (propValue !== true) {
      updatedCookie += "=" + propValue;
    }
  }

  document.cookie = updatedCookie;
}
function deleteCookie(name) {
  setCookie(name, "", {
    expires: -1
  })
}
if(getCookie('username')){
$.getScript('../users/'+getCookie('username')+'.js', function(){
	$("#ava").attr("src", profava); 
	$("#username").html(profname); 
	$("#title").html(proftitle); 
	$("#links").html('<a href="../profile/'+profname+'">Профиль</a> | <a href="../" id="leave">Выйти</a>'); 
	$("#leave").click(function(){
	deleteCookie('username');
});
});
} else {
	$("#ava").attr("src", ava); 
	$("#username").html(username); 
	$("#title").html(usertitle); 
	$("#links").html('<a href="../login">Войти</a>');
}
