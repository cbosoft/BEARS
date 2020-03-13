#include "sim.hpp"
#include "config.hpp"
#include "exception.hpp"
#include "formatter.hpp"
#include "progress.hpp"

struct BallConstructorData prep_bcd(const struct ConfigArgs &args)
{
  struct BallConstructorData bcd;
  bcd.id = 0;
  bcd.mass = args.mass;
  bcd.diameter = args.diameter;
  bcd.roughness = args.roughness;
  bcd.inertia = args.mass * args.diameter * args.diameter * 0.1;
  return bcd;
}

void simple_cubic_monodisperse_random_velocity(Sim &sim, const struct ConfigArgs &args)
{
  int n = args.n;
  int xn = ceil(cbrt(n));
  int yn = ceil(cbrt(n));
  int zn = ceil(cbrt(n));
  double sep = args.separation;
  sim.set_side_length(sep*((double)(xn+1)));
  
  struct BallConstructorData bcd = prep_bcd(args);

  ProgressBar pb(xn*yn*zn, "generating sc config", 1.0);
  for (int xi = 0; xi < xn; xi++) {
    for (int yi = 0; yi < yn; yi++) {
      for (int zi = 0; zi < zn; zi++) {
        bcd.id++;
        bcd.position = {sep*xi, sep*yi, sep*zi};
        bcd.velocity = vec_nrand(0, 1.0) * args.velocity_mag;
        bcd.angular_velocity = vec_nrand(0, 1.0) * args.angular_velocity_mag;
        sim.add_ball(bcd);

        pb.update(1);

      }
    }
  }
}

void interacting_pair(Sim &sim, const struct ConfigArgs &args)
{
  struct BallConstructorData bcd = prep_bcd(args);

  sim.set_side_length(args.separation*2);

  bcd.id++;
  bcd.position = {0.0, 0.0, 0.0};
  bcd.velocity = {args.velocity_mag, 0.0, 0.0};
  bcd.angular_velocity = {args.angular_velocity_mag, 0.0, 0.0};
  sim.add_ball(bcd);

  bcd.id++;
  bcd.position = {args.separation, 0.0, 0.0};
  bcd.velocity = {-args.velocity_mag, 0.0, 0.0};
  bcd.angular_velocity = {-args.angular_velocity_mag, 0.0, 0.0};
  sim.add_ball(bcd);

}

struct ConfigArgs default_args()
{
  struct ConfigArgs args = {
    .n=9, 
    .mass=1.0, 
    .diameter=1.0, 
    .separation=1.1, 
    .roughness=0.0, 
    .velocity_mag=1.0, 
    .angular_velocity_mag=1.0, 
    .path="config.tsv",
    .config_type="simple_cubic"
  };
  return args;
}

#define EITHER(A,B) ((strcmp(argv[i], A) == 0) || (strcmp(argv[i], B) == 0))
struct ConfigArgs parse_args(int argc, const char **argv)
{
  struct ConfigArgs args = default_args();
  --argc; ++argv;
  for (int i = 0; i < argc; i++) {
    if (EITHER("-n", "--number")) {
      args.n = atoi(argv[++i]);
    }
    else if (EITHER("-m", "--mass")) {
      args.mass = atoi(argv[++i]);
    }
    else if (EITHER("-d", "--diameter")) {
      args.diameter = atoi(argv[++i]);
    }
    else if (EITHER("-s", "--separation")) {
      args.separation = atoi(argv[++i]);
    }
    else if (EITHER("-r", "--roughness")) {
      args.roughness = atoi(argv[++i]);
    }
    else if (EITHER("-v", "--velocity")) {
      args.velocity_mag = atoi(argv[++i]);
    }
    else if (EITHER("-a", "--angular-velocity")) {
      args.angular_velocity_mag = atoi(argv[++i]);
    }
    else if (EITHER("-p", "--path")) {
      args.path = argv[++i];
    }
    else if (EITHER("-t", "--config-type")) {
      args.config_type = argv[++i];
    }
    else {
      throw ArgumentError(Formatter() << "Unknown arg \"" << argv[i] << "\".");
    }
  }

  return args;
}

void init_config(Sim &sim, const struct ConfigArgs &args)
{
  if (args.config_type.compare("simple_cubic") == 0) {
    simple_cubic_monodisperse_random_velocity(sim, args);
  }
  else if (args.config_type.compare("interacting_pair") == 0) {
    interacting_pair(sim, args);
  }
  else {
    throw ArgumentError(Formatter() << "Unknown config type \"" << args.config_type << "\".");
  }
}
