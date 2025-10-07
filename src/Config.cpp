#include "Config.hpp"
#include <fstream>
#include <iostream>

#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#else
#error "nlohmann/json.hpp not found. Please install nlohmann-json or provide the header."
#endif

static void from_json(const json &j, GridConfig &g) {
    if (j.contains("nx")) j.at("nx").get_to(g.nx);
    if (j.contains("ny")) j.at("ny").get_to(g.ny);
    if (j.contains("dx")) j.at("dx").get_to(g.dx);
    if (j.contains("dy")) j.at("dy").get_to(g.dy);
}

static void from_json(const json &j, MaterialBlock &m) {
    j.at("x0").get_to(m.x0);
    j.at("y0").get_to(m.y0);
    j.at("w").get_to(m.w);
    j.at("h").get_to(m.h);
    j.at("eps_r").get_to(m.eps_r);
}

static void from_json(const json &j, SourceConfig &s) {
    if (j.contains("type")) j.at("type").get_to(s.type);
    if (j.contains("x")) j.at("x").get_to(s.x);
    if (j.contains("y")) j.at("y").get_to(s.y);
    if (j.contains("amplitude")) j.at("amplitude").get_to(s.amplitude);
    if (j.contains("t0")) j.at("t0").get_to(s.t0);
    if (j.contains("spread")) j.at("spread").get_to(s.spread);
    if (j.contains("freq_hz")) j.at("freq_hz").get_to(s.freq_hz);
}

static void from_json(const json &j, MagnetConfig &m) {
    if (j.contains("x")) j.at("x").get_to(m.x);
    if (j.contains("y")) j.at("y").get_to(m.y);
    if (j.contains("moment_x")) j.at("moment_x").get_to(m.moment_x);
    if (j.contains("moment_y")) j.at("moment_y").get_to(m.moment_y);
    if (j.contains("strength")) j.at("strength").get_to(m.strength);
    if (j.contains("name")) j.at("name").get_to(m.name);
}

static void from_json(const json &j, VisualConfig &v) {
    if (j.contains("field")) j.at("field").get_to(v.field);
    if (j.contains("color_range")) j.at("color_range").get_to(v.color_range);
}

std::optional<Config> Config::loadFromFile(const std::string &path) {
    std::ifstream ifs(path);
    if (!ifs) {
        std::cerr << "Could not open config file: " << path << "\n";
        return std::nullopt;
    }
    json j;
    try {
        ifs >> j;
    } catch (std::exception &e) {
        std::cerr << "Failed to parse config: " << e.what() << "\n";
        return std::nullopt;
    }

    Config cfg;
    if (j.contains("grid")) from_json(j.at("grid"), cfg.grid);
    if (j.contains("timestepping") && j.at("timestepping").contains("max_steps"))
        cfg.max_steps = j.at("timestepping").at("max_steps").get<int>();
    if (j.contains("materials")) {
        for (auto &mi : j.at("materials")) {
            MaterialBlock m;
            from_json(mi, m);
            cfg.materials.push_back(m);
        }
    }
    if (j.contains("sources")) {
        for (auto &si : j.at("sources")) {
            SourceConfig s;
            from_json(si, s);
            cfg.sources.push_back(s);
        }
    }
    if (j.contains("magnets")) {
        for (auto &mi : j.at("magnets")) {
            MagnetConfig m;
            from_json(mi, m);
            cfg.magnets.push_back(m);
        }
    }
    if (j.contains("visualization")) from_json(j.at("visualization"), cfg.vis);
    if (j.contains("scenario")) j.at("scenario").get_to(cfg.scenario);

    return cfg;
}
