#include "ScorePsb.hpp"

namespace jian {

	REG_SCORER("psb", Score<CGpsb>);

	void Score<CGpsb>::init() {
		m_bin = 0.3;
		m_cutoff = 20;
		m_bins = int(std::ceil(m_cutoff / m_bin));
		read_counts();
		set_freqs();
	}

	void Score<CGpsb>::run(const Chain & c) {
		int i, j, l;
		Chain chain;

		chain = CGpsb::chain(c);
		l = chain.size();
		m_score = 0;
		for (i = 0; i < l; i++) {
			//m_score += en_stacking(chain[i], chain[j]);
			for (j = i + 1; j < l; j++) {
				m_score += en_pairing(chain[i], chain[j]);
			}
		}
	}

	void Score<CGpsb>::train(const Chain &c) {
		unsigned i, j, len;
		Chain chain;

		chain = CGpsb::chain(c);
		m_chain = &chain;
		set_indices();
		len = chain.size();
		for (i = 0; i < len - 1; i++) {
			update_counts_stacking(i, i + 1);
			for (j = i + 2; j < len; j++) {
				update_counts_pairing(i, j);
			}
		}
	}

	void Score<CGpsb>::set_indices() {
		unsigned i, len;

		len = m_chain->size();
		m_indices.resize(len);
		for (i = 0; i < len; i++) {
			m_indices[i] = m_map[m_chain->at(i).name];
		}
	}

	void Score<CGpsb>::update_counts_stacking(int n1, int n2) {
		unsigned i, j;
		double d;
		int n, a, b;

		a = m_indices[n1];
		b = m_indices[n2];
		const Residue &r1 = m_chain->at(n1);
		const Residue &r2 = m_chain->at(n2);
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				d = geom::distance(r1[i], r2[j]);
				if (d < 20) {
					n = (a * 3 + i) * 12 + (b * 3 + j);
					m_counts_stacking(n, int(d / m_bin))++;
				}
			}
		}
	}

	void Score<CGpsb>::update_counts_pairing(int n1, int n2) {
		unsigned i, j;
		double d;
		int n, a = m_indices[n1], b = m_indices[n2];
		const Residue &r1 = m_chain->at(n1);
		const Residue &r2 = m_chain->at(n2);
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				d = geom::distance(r1[i], r2[j]);
				if (d < 20) {
					n = (a * 3 + i) * 12 + (b * 3 + j);
					m_counts_pairing(n, int(d / m_bin))++;
				}
			}
		}
	}

	void Score<CGpsb>::read_counts(Mati & m, std::string path, int cutoff) {
		std::ifstream ifile;
		int i, j;
		
		FOPEN(ifile, path);
		m = Mati::Zero(144+1, m_bins);
		for (i = 0; i < 144+1; i++) {
			for (j = 0; j < m_bins; j++) {
				ifile >> m(i, j);
				if (j < cutoff) m(i, j) = 0;
			}
		}
		FCLOSE(ifile);
	}

	void Score<CGpsb>::read_counts() {
		read_counts(m_counts_stacking, Env::lib() + "/RNA/pars/scoring/score_psb/stacking.counts", 0);
		read_counts(m_counts_pairing, Env::lib() + "/RNA/pars/scoring/score_psb/pairing.counts", 0);
	}

	void Score<CGpsb>::set_freqs(Mat & m, const Mati & c) {
		Vec v;
		int i, j, sum;

		m = Mat::Zero(144, m_bins);
		for (i = 0; i < 144; i++) {
			sum = c.row(i).sum();
			for (j = 0; j < m_bins; j++) {
				m(i, j) = (sum == 0 ? 0 : (double(c(i, j)) / sum));
			}
		}

		v = Vec::Zero(m_bins);
		sum = c.row(144).sum();
		for (i = 0; i < m_bins; i++) {
			v[i] = double(c(144, i)) / sum;
		}

		for (i = 0; i < 144; i++) {
			for (j = 0; j < m_bins; j++) {
				m(i, j) = (v[j] > 0.0003 ? (m(i, j) / v[j]) : 0);
			}
		}
	}

	void Score<CGpsb>::set_freqs() {
		int i, j;

		for (i = 0; i < 144; i++) for (j = 0; j < m_bins; j++) m_counts_pairing(i, j) += m_counts_stacking(i, j);
		set_freqs(m_freqs_stacking, m_counts_stacking);
		set_freqs(m_freqs_pairing, m_counts_pairing);
	}

	double Score<CGpsb>::en_stacking(const Residue &r1, const Residue &r2) {
		double e, d, f;
		int a, b, t1, t2;
		const Residue *p1, *p2;
		Residue *temp1, *temp2;

		if (r1.is_cg<CGpsb>() && r2.is_cg<CGpsb>()) {
			temp1 = NULL;
			temp2 = NULL;
			p1 = &r1;
			p2 = &r2;
		}
		else {
			temp1 = new Residue(r1);
			temp2 = new Residue(r2);
			temp1->cg<CGpsb>();
			temp2->cg<CGpsb>();
			p1 = temp1;
			p2 = temp2;
		}

		e = 0;
		t1 = m_map[p1->name];
		t2 = m_map[p1->name];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				d = geom::distance(p1->at(i), p2->at(j));
				if (d < 20) {
					a = t1 * 3 + i;
					b = t2 * 3 + j;
					f = m_freqs_stacking(a * 12 + b, int(d / m_bin));
					if (f != 0) {
						e += -std::log(f);
					}
					else {
						e += 0;
					}
				}
			}
		}
		if (temp1 != NULL) delete temp1;
		if (temp2 != NULL) delete temp2;
		return e;
	}

	double Score<CGpsb>::en_pairing(const Residue &r1, const Residue &r2) {
		double e, d, f;
		int i, j, a, b, t1, t2;
		const Residue *p1, *p2;
		Residue *temp1, *temp2;

		if (r1.is_cg<CGpsb>() && r2.is_cg<CGpsb>()) {
			temp1 = NULL;
			temp2 = NULL;
			p1 = &r1;
			p2 = &r2;
		}
		else {
			temp1 = new Residue(r1);
			temp2 = new Residue(r2);
			temp1->cg<CGpsb>();
			temp2->cg<CGpsb>();
			p1 = temp1;
			p2 = temp2;
		}

		t1 = m_map[r1.name];
		t2 = m_map[r2.name];
		e = 0;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				d = geom::distance(r1[i], r2[j]);
				if (d < m_cutoff) {
					a = t1 * 3 + i;
					b = t2 * 3 + j;
					f = m_freqs_pairing(a * 12 + b, int(d / m_bin));
					e += (f == 0 ? 0 : -std::log(f));
				}
			}
		}
		if (temp1 != NULL) delete temp1;
		if (temp2 != NULL) delete temp2;
		return e;
	}

	void Score<CGpsb>::print_counts(std::ostream & stream, const Mati & c) const {
		stream << c << std::endl;
	}

	void Score<CGpsb>::print_counts(std::ostream & stream) const {
		stream << "Stacking counts: " << std::endl;
		print_counts(stream, m_counts_stacking);
		stream << "Pairing counts: " << std::endl;
		print_counts(stream, m_counts_pairing);
	}

	void Score<CGpsb>::print_freqs(std::ostream & stream, const Mat & f) const {
		stream << f << std::endl;
	}

	void Score<CGpsb>::print_freqs(std::ostream & stream) const {
		stream << "Stacking freqs: " << std::endl;
		print_freqs(stream, m_freqs_stacking);
		stream << "Pairing freqs: " << std::endl;
		print_freqs(stream, m_freqs_pairing);
	}



} // namespace jian

