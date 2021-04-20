
# Distributions

The different types of distributions that could be used.

E.g. 

* kernel based
* histogram

The distributions are created using settings specific to them, the settings for each distribution
are stored in the distribution_settings folder, to provide a common interface the distribution_settings
base class can be passed into the factory. Once the create method in the distribution is called it can
be cast back to the appropriate type. 
