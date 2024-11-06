module Boids
using Plots

mutable struct WorldState
    boids::Vector{Tuple{Float64, Float64}}
    velocities::Vector{Tuple{Float64, Float64}}
    v_min::Float64
    v_max::Float64
    height::Float64
    width::Float64
    radius::Float64
    coherence::Float64
    separation::Float64
    alignment::Float64
    function WorldState(n_boids, h, w, v_min, v_max, radius, coherence, separation, alignment)
        boids = [(rand(0:w), rand(0:h)) for _ in 1:n_boids]
        velocities = [(rand() * 2 - 1, rand() * 2 - 1) for _ in 1:n_boids]
        new(boids, velocities, v_min, v_max, h, w, radius, coherence, separation, alignment)
    end
end

function distance(boid1::Tuple{Float64, Float64}, boid2::Tuple{Float64, Float64})
    return sqrt((boid2[1] - boid1[1])^2 + (boid2[2] - boid1[2])^2)
end

function average(boids::Vector{Tuple{Float64, Float64}})
    sum_x = 0.0
    sum_y = 0.0
    n = length(boids)

    for boid in boids
        sum_x += boid[1]  
        sum_y += boid[2]  
    end
    avg = (sum_x / n, sum_y / n)
    return avg
end

function coherence(state::WorldState)
    v_coherence = Vector{Tuple{Float64, Float64}}(undef, length(state.boids))
    for i in 1:length(state.boids)
        mates::Vector{Tuple{Float64, Float64}} = []
        for j in 1:length(state.boids)
            if (i != j) & (distance(state.boids[i], state.boids[j]) < state.radius)
                push!(mates, state.boids[j])
            end
        end
        if isempty(mates)
            v_coherence[i] = (0.0, 0.0)
        else
            v_coherence[i] = state.coherence .* (average(mates) .- state.boids[i])
        end
    end
    return v_coherence
end   

function separation(state::WorldState)
    v_separation = Vector{Tuple{Float64, Float64}}(undef, length(state.boids))
    for i in 1:length(state.boids)
        mates::Vector{Tuple{Float64, Float64}} = []
        v_separation[i] = (0.0, 0.0)
        for j in 1:length(state.boids)
            if (i != j) & (distance(state.boids[i], state.boids[j]) < state.radius)
                v_separation[i] = v_separation[i] .+ (state.separation ./ (state.boids[i] .- state.boids[j]))
            end
        end
    end
    return v_separation
end 

function alignment(state::WorldState)
    v_alignment = Vector{Tuple{Float64, Float64}}(undef, length(state.boids))
    for i in 1:length(state.boids)
        mates::Vector{Tuple{Float64, Float64}} = []
        for j in 1:length(state.boids)
            if (i != j) & (distance(state.boids[i], state.boids[j]) < state.radius)
                push!(mates, state.velocities[j])
            end
        end
        if isempty(mates)
            v_alignment[i] = (0.0, 0.0)
        else
            v_alignment[i] = state.alignment .* (average(mates) .- state.velocities[i])
        end
    end
    return v_alignment
end 

function separation_walls(state::WorldState)
    v_separation_walls = Vector{Tuple{Float64, Float64}}(undef, length(state.boids))
    for i in 1:length(state.boids)
        near_zero_x = state.boids[i][1] < state.radius[1]
        near_zero_y = state.boids[i][2] < state.radius[1]
        near_w = state.boids[i][1] > (state.width[1] - state.radius[1])
        near_h = state.boids[i][2] > (state.height[1] - state.radius[1])
        v_separation_walls[i] = 1 .* ((near_zero_x || near_w) / (state.boids[i][1] .- (near_w * state.width[1])), 
        (near_zero_y || near_h) / (state.boids[i][2] .- (near_h * state.height[1])))
    end
    return v_separation_walls
end

function update!(state::WorldState)
    v_coherence = coherence(state)
    v_alignment = alignment(state)
    v_separation = separation(state)
    v_separation_walls = separation_walls(state)
    for i in 1:length(state.boids)
        state.velocities[i] = state.velocities[i] .+ v_coherence[i] .+ v_alignment[i] .+ v_separation[i] .+ v_separation_walls[i]
        if sqrt(state.velocities[i][1]^2 + state.velocities[i][2]^2) > state.v_max[1]
            state.velocities[i] = state.velocities[i] .* (state.v_max[1] / sqrt(state.velocities[i][1]^2 + state.velocities[i][2]^2))
        end
        state.boids[i] = state.boids[i] .+ state.velocities[i]
    end
    
    # TODO: реализация алгоритма
    return nothing
end

function (@main)(ARGS)
    h, w = 50, 50
    n_boids = 60
    v_min::Float64, v_max::Float64 = 0, 1 # Максимальная скорость должна быть меньше радиуса, чтобы боиды не могли преодолеть границы карты
    radius::Float64 = 5
    ch::Float64, sp::Float64, al::Float64 = 0.07, 0.005, 0.005

    state = WorldState(n_boids, h, w, v_min, v_max, radius, ch, sp, al)

    anim = @animate for time = 1:100
        update!(state)
        boids = state.boids
        scatter(boids, xlim = (0, state.width), ylim = (0, state.height))
    end
    gif(anim, "boids.gif", fps = 24)
end

end

using .Boids
Boids.main("")
