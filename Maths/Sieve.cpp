#include "../Defines.h"

/* START SOLUTION */

ll MAX_PRIME = 110267;
vl PRIMES;
vb IS_PRIME(MAX_PRIME + 1, true);

// MAX_PRIME = 10^6 takes 0.2s
// MAX_PRIME = 10^8 takes 2s
// MAX_PRIME = 10^9 takes 18s 
// # of primes < N is O(N / lgN)
void run_sieve() {
  IS_PRIME[0] = IS_PRIME[1] = false;
  // if you don't need PRIMES (only IS_PRIME), limit i
  // to less than (int)sqrt(MAX_PRIME) + 1
  // IS_PRIME will still be correctly populated
  for(ll i = 2; i <= MAX_PRIME; ++i) {
      if(!IS_PRIME[(int)i]) continue;

      PRIMES.push_back(i);
      for(ll j = i * i; j <= MAX_PRIME; j += i)
        IS_PRIME[(int)j] = false;
      }
}

// only use after run_sieve
// to find prime factors of n, you need only calculate
// primes up to sqrt(n) (as we can't have more than 1
// prime > sqrt(n) in the prime factorisation)
void prime_facts(ll n, vl& pfs) {
  int p = 0;
  ll pfact = PRIMES[0];
  while(n != 1 && p < PRIMES.size() && pfact * pfact <= n) {
      while(n % pfact == 0) {
        n /= pfact;
        pfs.push_back(pfact);
      }

      pfact = PRIMES[++p];
  }

  if(n != 1) pfs.push_back(n);
}

// only use after run_sieve
// returns the number of natural numbers less than or equal
// to n that are coprime to n
// tot(n) = Product( p_i ^ (e_i - 1) * (p_i - 1) ) where
//          p_i appears with multiplicity e_i in prime 
//          factorisation of n
// tot(n) = n Product( 1 - 1/p ) where p ranges across all
//          primes dividing n
ll totient(ll n) {
    vl pfs;
    prime_facts(n, pfs);
    ll tot = 1;
    ll cur_run = 0;
    for(int p = 0; p < pfs.size(); ++p) {
        ++cur_run;

        if(p == pfs.size() - 1 || pfs[p + 1] != pfs[p]) {
            while(--cur_run != 0) tot *= pfs[p];
            tot *= (pfs[p] - 1);
        }
    }

    return tot;
}

/* END SOLUTION */

// solves:
// http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=230
// with MAX_PRIME = 31623
int solve_divisors() {
  run_sieve();

  int N;
  cin >> N;

  for(int n = 0; n < N; ++n) {
  ll L, U;
  cin >> L >> U;
  ll ma = -1, mac = -1;
  for(int c = L; c <= U; ++c) {
    vl pfs, ns;
    prime_facts(c, pfs);
    for(int i = 0; i < pfs.size(); ++i)
    if(i == 0 || pfs[i] != pfs[i - 1]) ns.push_back(1);
    else ++ns.back();
    ll nfacts = 1;
    for(ll np : ns) nfacts *= np + 1;
    
    if(nfacts > ma) {
    ma = nfacts;
    mac = c;
    }
  }

  cout << "Between " << L << " and " << U << ", " << mac
     << " has a maximum of " << ma << " divisors.\n";
  }
  return 0;
}

// solves:
// http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=524
// with MAX_PRIME = 46342
int solve_prime_factors() {
  run_sieve();

  while(true) {
    ll n;
    cin >> n;

    if(n == 0) break;

    vl pfs;
    prime_facts(abs(n), pfs);

    cout << n << " = ";
    if(n < 0) cout << "-1 x ";
    for(int i = 0; i < pfs.size(); ++i) {
      if(i > 0) cout << " x ";
      cout << pfs[i];
    }
    cout << endl;
  }

  return 0;
}

int simple_tot_test() {
    run_sieve();
    cout << "phi(1) = " << totient(1) << endl;
    cout << "phi(2) = " << totient(2) << endl;
    cout << "phi(6) = " << totient(6) << endl;
    cout << "phi(10) = " << totient(10) << endl;
    cout << "phi(72) = " << totient(72) << endl;

    return 0;
}

// solve:
// http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=&problem=2302
ll gcd(ll a, ll b) { return b != 0 ? gcd(b, a % b) : a; }

int solve_enum_rationals() {
    run_sieve();

    ll tot_count = 0;
    ll denom = 1;
    vl counts = {0};
    while(tot_count <= 12158598919ll) {
        tot_count += totient(denom);
        counts.push_back(tot_count);
        ++denom;
    }

    while(true) {
        ll m;
        cin >> m;

        if(m == 0) break;
        else if(m == 1) cout << "0/1\n";
        else if(m == 2) cout << "1/1\n";
        else {
            --m;
            ll d = 1;
            while(m > counts[d]) ++d;
            m -= counts[d - 1];

            for(int f = 1; f < d; ++f) {
                if(gcd(f, d) == 1 && --m == 0) {
                    cout << f << "/" << d << "\n";
                    break;
                }
            }
        }
    }
}

int main() {
  return solve_enum_rationals();
}
