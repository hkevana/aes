#ifndef FLAGS_H
#define FLAGS_H

class flags {
	public:
		static bool verbose;
		static bool encrypt;
		static bool decrypt;
		static bool outfile;
		static bool pause;

		flags();
		~flags();
};
#endif

