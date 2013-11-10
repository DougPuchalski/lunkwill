ModuleName="LuaDummy"
ModuleDescription="Dummy LUA Module"

function ModuleAnswerRequest(user, group, session1, session2, project, module, module_request)
        return 0, "Test", module_request
end