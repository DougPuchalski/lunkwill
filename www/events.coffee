window.onhashchange =  ->
	a=window.loading
	window.loading=true
	b=location.hash.split('/')[1]
	c=location.hash.split('/')[2]
	d=location.hash.split('/')[3]
	window.setCookie "project", b
	window.setCookie "module",  c
	window.loading = a
	window.ajax( atob d ) if window.loading is false

window.onload = ->
	CSS.flush()
	window.ajax ""


