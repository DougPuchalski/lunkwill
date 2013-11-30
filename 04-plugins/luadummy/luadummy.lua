ModuleName="LuaDummy"
ModuleDescription="Dummy LUA Module"

function ModuleAnswerRequest(user, group, session1, session2, project, module, module_request)
        return 0, "Sidebar", "LUA ARGS: USER:"..user.." GROUP:"..group.." SESSION1:"..session1.." SESSION2:"..session2.." PROJECT:"..project.." MODULE:"..module.." MODULE_REQUEST:"..module_request
end
