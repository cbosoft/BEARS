# ![icon](https://github.com/cbosoft/BEARS/blob/master/assets/koala.png?raw=true) BEARS v0.1

An event driven molecular dynamics program (WIP). Event driven dynamic simulation is a fast simulation technique by way of looking for the next interaction for each particle, resolving the molecular dynamic simulation to a simple sorting issue. This has the added advantage of modelling perfecty hard sphere particles *with no force field necessary*.

The goal of this package is to perform simple ballistic or fluid mediated simulation of a **very large** system of hard spheres. Previous codes I have worked on have only been efficient enough to deal with O(10^3) particles in decent time. For this project, I want to be able to simulate at **O(10^6)**. The key will be parallelisation and giving agency to the particle.

## Features and wishlist
  - [x] Plain ballistic simulation of particles by impulsive interaction.
  - [x] Simple parallel acceleration.
  - [x] Trajectory logging (to tab separated value, .tsv).
  - [ ] Heavier parallelisation, more work dished out to threads and off the main thread.
  - [ ] Frictional interaction.
  - [ ] Interstitial fluid interaction.
  - [X] Acceptable speed (< 1min/unit) up to 10^3 particles
  - [ ] Acceptable speed (< 1min/unit) up to 10^4 particles
  - [ ] Acceptable speed (< 1min/unit) up to 10^5 particles
  - [ ] Acceptable speed (< 5min/unit) up to 10^6 particles
  - [ ] Acceptable speed (< 10min/unit) up to 10^7 particles
  
## WIP: Watch this space.

<sub>Icon by [Smashicons](https://www.flaticon.com/authors/smashicons).</sub>
