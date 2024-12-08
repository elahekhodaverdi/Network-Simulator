
## DataGenerator Class

The `DataGenerator` class is responsible for generating data values based on the Pareto distribution. The class is designed to provide flexibility in initialization and data generation parameters.


### Fields

- **`m_minValue`**: A private member variable of type `int` that specifies the minimum value for the Pareto distribution. Defaults to `1`.

### Method Explanations

1. **Constructor**
    - **Default**: Initializes the `DataGenerator` object with default values (`m_minValue` = 1).

    - **Parameterized**: Initializes the `DataGenerator` with a user-defined minimum value for the Pareto distribution.

2. **`generateParetoPacketCounts`**  
   - **Purpose**: Generates packet counts following the Pareto distribution using the formula:  
     $$
     X = \frac{\text{minValue}}  {(1 - U)^{\frac{1}{\alpha}}}
     $$  
     where $U$ is a uniformly distributed random value between 0 and 1, and $\alpha$ is the Pareto shape parameter.  
   - **Details**:  
   We'll discuss a step-by-step explanation of the formula used in `generateParetoPacketCounts` and how it was derived just below.


### Understanding the Pareto Distribution Formula

The probability density function (PDF) is given by:

$$
f(x) = \alpha \frac{{x_m^\alpha}}{{x^{\alpha + 1}}}, \quad x \geq x_m, \, \alpha > 0
$$

Here:
- $x$: The variable of interest.
- $x_m$: The minimum value of $x$ (also called the scale parameter).
- $\alpha$: The shape parameter, controlling the "heaviness" of the distribution tail.

From the PDF, we derive the cumulative distribution function (CDF), which gives the probability that a random variable $X$ is less than or equal to some value $x$:

$$
F(x) = 1 - \left(\frac{x_m}{x}\right)^\alpha, \quad x \geq x_m
$$

To generate a random number following this distribution, we use the inverse transform sampling method.

### Inverse Transform Sampling

The key idea of inverse transform sampling is:
1. Start with a random number $U$ uniformly distributed in \([0, 1]\).
2. Transform $U$ using the inverse of the CDF $F(x)$ to produce a random value $X$ following the desired distribution.

For the Pareto distribution, the CDF is:

$$
F(x) = 1 - \left(\frac{x_m}{x}\right)^\alpha
$$

Setting $F(x) = U$, we get:

$$
U = 1 - \left(\frac{x_m}{x}\right)^\alpha
$$

Rearranging for $x$:
$$
x = x_m \cdot (1 - U)^{-1/\alpha}
$$

This formula allows us to generate a random value $x$ following the Pareto distribution given:

- $U$: A uniformly distributed random value.
- $x_m$: The minimum value (scale parameter).
- $\alpha$: The shape parameter.
