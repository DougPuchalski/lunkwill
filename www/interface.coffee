class window.main_interface
	title = "Interface"
	page = ""
	
	sidebarTitle = "Actions"
	sidebarPage = ""
	
	build: ->
		site = "<h1>" + title + "</h1>"
		document.getElementById("MAIN").innerHTML = site
		
		# sidebar =  ...etc

	setTitle: (t) ->
		title = t
	
	addContent: (c) ->
		page += c
	
	setSidebarTitle: (t) ->
		sidebarTitle = t
	
	addSidebarContent: (c) ->
		sidebarPage += c


	addPageCell: (data) ->
		#git_c.class='Cell'  <----- aka CommitCell
		#git_c_scale=document.createElement("div")
		
		#git_c_scale.innerHTML=data.replace("<","&lt;").replace(">","&gt;")

		#git_c.appendChild git_c_scale


		page += "<div class='cell'>" + data + "</div>"

	endPageCell: ->
		page += "</div>"

	startPageCol: (data)->
		page += "<div class="Col">
	
	endPageCol: ->
		page += "</div>"


	# Unused code...

	#git_row=(i)->
	#git_r.class='CommitRow'
	#color = "#EEE"
	#color = "#FFF" if (i%2 == 0)
	#git_r.setAttribute 'style', 'display:table-row;background:'+color+';'
	#git_r.setAttribute 'onclick', 'window.expand(this)'

	#git_r.appendChild git_cell window.repo.sha[i], 10
	#git_r.appendChild git_cell atob(window.repo.message[i]), 49
	#split= window.repo.author[i].split(" ")
	#git_r.appendChild git_cell atob(split[0])+" <"+atob(split[1])+">", 30
	#git_r.appendChild git_cell window.repo.time[i], 10

	#window.expand git_r
	#return git_r 
