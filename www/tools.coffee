expand_children=(c,d)->
	c.style.overflow='hidden'
	if !d
		for i in c.childNodes when i? && i.style?
			i.style.height = "20px"
			expand_children(i,d)
	else
		for i in c.childNodes when i? && i.style?
			i.style.height = "auto"
			expand_children(i,d)

window.expand = (me) ->
	me.elapsed ?= true
	if me.elapsed
		me.style.overflow='hidden'
		me.style.height = "20px"
		me.elapsed = false
		expand_children me, false
	else
		me.style.height = "auto"
		me.elapsed = true
		expand_children me, true

