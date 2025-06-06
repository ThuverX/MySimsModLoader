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

    function EA:ShouldOutput(class,level)
        return true
    end

    function LogLevel(class, level, ... )
        if level ~= 25 then
            print("[" .. class .. " " .. levels[level] .. "]", unpack(arg))
        end
    end

    function EA:Log(class, ... )
        LogLevel(class, self._defaultLogLevel, unpack(arg))
    end

    function EA:Trace( class, ... )
        LogLevel(class, self._defaultLogLevel, unpack(arg))
    end

    function EA:LogI( class, ... )
        LogLevel(class, EA.kLevelInfo, unpack(arg))
    end

    function EA:LogD( class, ... )
        LogLevel(class, EA.kLevelDebug, unpack(arg))
    end


    function EA:LogW( class, ... )
        LogLevel(class, EA.kLevelWarn, unpack(arg))
    end

    function EA:LogE( class,... )
        LogLevel(class, EA.kLevelError, unpack(arg))
    end
end