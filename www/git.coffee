git_cell=(data)->
	git_c=document.createElement("div")
	git_c.class='CommitCell'
	git_c.setAttribute 'style', 'display:table-cell;word-wrap:break-word;overflow:hidden;border: 1px solid black;'

	git_c_scale=document.createElement("div")
	git_c_scale.innerHTML=data.replace("<","&lt;").replace(">","&gt;")

	git_c.appendChild git_c_scale

	return git_c

git_row=(i)->
	git_r=document.createElement("div")
	git_r.class='CommitRow'
	color = "#EEE"
	color = "#FFF" if (i%2 == 0)
	git_r.setAttribute 'style', 'display:table-row;background:'+color+';'
	git_r.setAttribute 'onclick', 'window.expand(this)'

	git_r.appendChild git_cell window.repo.sha[i], 10
	git_r.appendChild git_cell atob(window.repo.message[i]), 49
	split= window.repo.author[i].split(" ")
	git_r.appendChild git_cell atob(split[0])+" <"+atob(split[1])+">", 30
	git_r.appendChild git_cell window.repo.time[i], 10

	window.expand git_r
	return git_r 

git_col=(width)->
	git_c=document.createElement("div")
	git_c.class='CommitCol'
	git_c.setAttribute 'style', 'display:table-column;word-wrap:break-word;width:'+width+';'
	return git_c


window.git_list = ->
	git_table=document.createElement("div")
	git_table.id='CommitList'
	git_table.setAttribute 'style', 'display:table; table-layout: fixed; width:100%;border: 1px solid black;'

	git_table.appendChild git_col "10%"
	git_table.appendChild git_col "auto"
	git_table.appendChild git_col "auto"
	git_table.appendChild git_col "10%"

	for sha,i in window.repo.sha
		git_table.appendChild git_row i
		
	window.EvalScriptElem.parentNode.insertBefore git_table, window.EvalScriptElem

