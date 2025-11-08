-- safety_trigger.lua
-- Module phản xạ an toàn cho hệ thống AIForDriver

local SafetyTrigger = {}

-- Ngưỡng kích hoạt khẩn cấp
SafetyTrigger.thresholds = {
    collision_distance = 5.0,  -- m
    fatigue_limit = 0.8,       -- mức mệt mỏi
    attention_drop = 0.2,      -- mức chú ý thấp
    sensor_loss_time = 1.5     -- giây không có dữ liệu mới
}

-- Hàm kiểm tra và kích hoạt khẩn cấp
function SafetyTrigger:evaluate(sensor_data, driver_status, last_update_time)
    local now = os.clock()
    local time_since_update = now - (last_update_time or now)

    if (sensor_data["distance_to_object"] or 999) < self.thresholds.collision_distance then
        self:trigger("POTENTIAL_COLLISION")
    elseif driver_status["fatigue"] and driver_status["fatigue"] > self.thresholds.fatigue_limit then
        self:trigger("DRIVER_FATIGUE")
    elseif driver_status["attention"] and driver_status["attention"] < self.thresholds.attention_drop then
        self:trigger("LOW_ATTENTION")
    elseif time_since_update > self.thresholds.sensor_loss_time then
        self:trigger("SENSOR_TIMEOUT")
    end
end

-- Hàm gửi tín hiệu khẩn cấp (giả lập gửi tới EmergencyProtocol.cpp)
function SafetyTrigger:trigger(reason)
    print(string.format("[SafetyTrigger] 🚨 Emergency Triggered: %s", reason))
    -- Giả lập gọi sang C++: EmergencyProtocol::trigger(reason)
    -- Trong thực tế có thể dùng bridge C/Lua
end

return SafetyTrigger

