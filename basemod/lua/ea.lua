if EA then

    local levels = {
        [1] = "ALL",
        [2] = "MIN",
        [25] = "DEBUG",
        [50] = "INFO",
        [100] = "WARN",
        [150] = "ERROR",
        [200] = "FATAL",
        [250] = "MAX",
        [251] = "NONE"
    }

    function EA:LogLevel(class, level, ... )
        print("[" .. levels[level] .. "]", unpack(arg))
    end
end