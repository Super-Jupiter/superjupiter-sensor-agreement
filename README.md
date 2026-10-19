# SuperJupiter SensorAgreement

Age-aware comparison of redundant sensors without inventing ground truth.

**Version 0.1.0: evolving preview. Hardware validation pending.**

Original MIT-licensed portable C++11 by [SuperJupiter](https://www.superjupiter.net).
No mandatory service, hidden clock reads, network calls or heap allocation in the core.

## What it does

Caller supplies values and ages for simultaneous comparison in common physical units.
Non-finite or stale samples are excluded; at least minSensors eligible values are required.
Reports mean, spread, eligible/excluded counts, and agreement against absoluteTolerance + relativeTolerance*max(abs(values)).
Agreement does not establish accuracy or identify the faulty sensor. No majority voting, automatic substitution or sensor health certification.
Bounded by caller-provided input length; no allocation, state or retained pointers.

## Install

PlatformIO exact release (after registry publication):

```ini
lib_deps = superjupiter/SuperJupiterSensorAgreement@0.1.0
```

Arduino: use the versioned [GitHub release ZIP](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/releases/tag/v0.1.0).
Library Manager registration and index propagation are separate steps; check the official index before relying on IDE availability.

## Example

```cpp
#include <SuperJupiterSensorAgreement.h>
using namespace superjupiter::sensor_agreement;
int main() {
Sample sensors[]={{20,0},{20.2,100},{19.9,50}};
Result agreement=compare(sensors,3,Config(0.5,0,1000,2));
(void)agreement;
return 0;
}

```

For Arduino, open `examples/Minimal`. It uses synthetic data only.
Results are deliberately local so applications can inspect outcomes before adding I/O.

## CMake

```cmake
add_subdirectory(path/to/superjupiter-sensor-agreement)
target_link_libraries(your_app PRIVATE SuperJupiter::SensorAgreement)
```

For an installed package use `find_package(SuperJupiterSensorAgreement 0.1 CONFIG REQUIRED)`.
Top-level CMake builds tests/examples; a subproject does not enable developer tools by default.

## Documentation and verification

- [API and boundaries](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/blob/v0.1.0/docs/API.md)
- [Tests](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/blob/v0.1.0/tests/test.cpp)
- [Validation status](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/blob/v0.1.0/docs/VALIDATION.md)
- [Shared conventions](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/blob/v0.1.0/docs/CONVENTIONS.md)
- [Migration policy](https://github.com/Super-Jupiter/superjupiter-sensor-agreement/blob/v0.1.0/docs/MIGRATION.md)

```sh
cmake -S . -B build/cmake
cmake --build build/cmake
ctest --test-dir build/cmake --output-on-failure
python3 tools/check_release.py --tag v0.1.0
```

Keep raw observations and their provenance in your application. Derived results are not physical truth.
Do not use fast-math or finite-math-only compiler assumptions. AVR double may have float precision.
Software tests, board compilation and physical validation are reported separately.

## Fits the SuperJupiter measurement portfolio

Complements [SensorQuality](https://github.com/Super-Jupiter/superjupiter-sensor-quality) and
[SampleWindow](https://github.com/Super-Jupiter/superjupiter-sample-window) without a required dependency.
Each component can be used independently. Domain models, customer data and proprietary applications are excluded.

## Support and licence

MIT. Report reproducible bugs through repository issues; private security reports: info@superjupiter.net.
Include version, configuration, inputs, expected and observed behaviour. No credentials or private customer data.
