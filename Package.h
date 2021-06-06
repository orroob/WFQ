
class Connection
{
public:
	char Sadd[16], Dadd[16];
	int Sport, Dport;
public:
	Connection() = default;

	friend bool operator==(const Connection& p1, const Connection& p2);
	friend std::ostream& operator<<(std::ostream& os, const Connection& p);
};

typedef class Package : public Connection
{
public:
	int time, len;
	double weight = 1;

public:
	Package() = default;

	friend std::ostream& operator<<(std::ostream& os, const Package& p);
}pck;

std::ostream& operator<<(std::ostream& os, const Package& p)
{
	if (p.weight != 1)
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len << ' ' << std::fixed << std::setprecision(2) << p.weight;
	else
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Connection& p)
{
	os << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ';// << p.len;
	return os;
}

bool operator==(const Connection& p1, const Connection& p2)
{
	return (!strcmp(p1.Sadd, p2.Sadd) && !strcmp(p1.Dadd, p2.Dadd) && p1.Sport == p1.Sport && p1.Dport == p2.Dport);
}
