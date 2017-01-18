#include "nsp.hpp"

BEGIN_JN

NSP &NSP::instance() {
	static NSP nsp;
	return nsp;
}

void NSP::run(int argc, char **argv) {
	Par par(argc, argv);
	NSP::instance().m_argc = argc;
	NSP::instance().m_argv = argv;
	if (par.has("log_level")) {
		log_level(std::stoi(par["log_level"][0]));
	}
	if (par.has("log")) {
		log_file(par.get("log"));
	}
	if (par.has("out", "o")) {
		out_file(par.get("out", "o"));
	}
	auto &m = instance()._methods;
	S path = Env::lib() + "/RNA/pars/src/";
	if (par._orig_pars.size() <= 1 || m.find(par[1]) == m.end()) {
		S name = path + "nsp.md";
		for (auto &&it : FileLines(name)) {
			std::cout << it.line << std::endl;
		}
		for (auto && pair : m) { std::cout << pair.first << ' '; }
		std::cout << std::endl;
	}
	else if (par.has("help") || par.has("h") || par.has("-help")) {
		S name = path + par[1] + ".md";
		for (auto &&it : FileLines(name)) {
			std::cout << it.line << std::endl;
		}
	}
	else {
		m[par[1]](par);
	}
}

END_JN

