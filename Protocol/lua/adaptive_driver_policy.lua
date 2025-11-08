-- adaptive_driver.lua 
-- Bo dieu khien thich nghi cho he thong lai 

local AdaptiveDriver = {}

-- Tham so dieu chinh dong (adaptive parameters)

AdaptiveDriver.params = 
{
    base_speed = 60, 
    max_acceleration = 3.5, 
    min_follow_distance = 15, 
    reactivity = 0.8 
}

-- Bo nho trang thai ngan han (short- term memory).. 
AdaptiveDriver.state = 
{
    last_action = "NONE", 
    fatigue_level = 0.0, 
    attention_level = 1.0 
}

-- Ham cap nhat du lieu tu sensor_sync ... 
function AdaptiveDriver :update(sensor_data, driver_status)
    local road_condition = sensor_data["road_condition"] or 1.0 
    local traffic_density = sensor_data["traffic_density"] or 0.5 
    local attention = driver_status["attention"] or 1.0 
    local fatigue = driver_status["fatigue"] or 0.0 

    self.state.fatigue_level = fatigue 
    self.state.attention_level  = attention

    -- dieu chinh hanh vi dua tren tinh huong -- 
    local adjusted_speed = self.params.base_speed  *road_condition *(1.0 - fatigue)
    local follow_distance = self.params.min_follow_distance *(1.0 + traffic_density )

    -- Tang toc/ giam toc thich ung .. // 
    local accel = (attention > 0.7) and self.params.max_acceleration 

    print(string.format("[AdaptiveDriver] Speed: %.1f km/h, Distance: %.1f m, Accel: %.2f"),
    adjusted_speed, follow_distance, accel)

    -- Gui lenh dieu khien (mo phong gui sang Control Channel)
    self.state.last_action = "SET_SPEED"
    return 
    {
        command = "SET_SPEED", 
        value = adjusted_speed, 
        timestamp = os.clock()
    }
end 

-- ham tu hoc don gian (co the mo rong sang reiforcement learning).. 
function AdaptiveDriver : learn(feedback)
    if not feedback then return end 
    if feedback == "TOO FAST" then 
        self.params.base_speed = self.params.base_speed -2 
    elseif feedback =="TOO_SLOW" then
        self.params.base_speed = self.params.base_speed + 2 
    elseif feedback == "UNSTABLE" then
        self.params.reactivity = math.max(0.5, self.params.reactivity -0.1)
    end 
    print("[AdaptiveDriver] Adjusted parameters -> Base Speed:", self.params.base_speed)
end
