## Dataset
In this project, we are working with a [dataset](https://www.kaggle.com/code/amirmotefaker/supply-chain-analysis/input) from a beauty and makeup supplier.
The dataset was publicly available but contained unrealistic values, so I refined and adjusted the data to better reflect plausible industry scenarios before applying it in my optimization models.
The refined dataset is available [here](https://github.com/sfzgzs/gurobi-notes/blob/main/makeup-supply-chain-project/code/cleaned_supply_chain_data.csv).
(The code used for refining the database is included in the project)
### Data cleaning and Adjustments 
- For the fields **price**, **revenue generated**,**shipping cost**, and **manufacturing cost**, the data is supposed to be in some currency.
  It’s unusual for product prices to have more than 2 decimal places since pricing reflects currency.
    - This issue is fixed by limiting these numbers to two 2 decimal places.
- The value of the **manufacturing cost** should normally not be less than the **price**.
  In fact, in most cases, the (selling) **price** should be more than **manufacturing cost**+**shipping cost** for the business to gain any profit at all.
  It is also common sense that Shipping Cost should be $\leq$ 25% of Selling Price.
    - This is fixed by recalculating the Price (in rows in the dataset that do not make sense) as
      $(Manufacturing Cost+Shipping Cost)×(1+Markup Percentage)$
      where the typical **markup percentage** for mass-market beauty products is a ratio between [1,3].
- The field **revenue generated** is not equal to **price**$\times$**number of products sold**. This can easily be fixed.
- Excessively high **defect rates** are reported. Typical industry defect rates are <1%. Easy fix.
- **Manufacturing lead times** range from 1 to 30 days, with no correlation to the supplier. (I did not change this)
- In realistic supply chain timelines, **manufacturing lead time for cosmetics** is typically between 3–15 days and **shipping lead time** is 2–7 days for domestic shipments, and 7–30 days for international shipments.
  - Could be fixed using ranged random generation.
- There are two fields with "lead time" header...
  - Removed one.
- Some rows have **Pass** as **inspection result** with high Defect Rates. Normally, if Defect Rate > threshold, the inspection would fail.
  - Fixed it to if Defect Rate > 2%, force Inspection Result = "Fail".
- **Transportation modes** and **shipping times** mismatch. Some products shipped by **Air** have long lead times (e.g., 30 days) even though **Air** is usually used for fast delivery.
  - Fix for **Air** shipments: **Lead Time** $\leq$ 7 days and **sea shipments**: **Lead Time** $\geq$ 15 days.
---
## Problem Statement
In this dataset, we have one carrier(out of A, B, and C) selected for each product. 
Now, let us consider a situation where **all carriers offer identical services** for each SKU (exact same routes, modes, and lead times), but with **different shipping costs** (randomized by ±0.05 of the base cost).  

Now, the reason the store would not immediately choose the carrier with the minimum cost of shipping for each product is that bulk discounts apply when total shipped packages exceed a certain threshold for each carrier:
- **Carrier A**: 25% off beyond **2500 packages**
- **Carrier B**: 30% off beyond **2600 packages**
- **Carrier C**: 20% off beyond **2400 packages**

Our goal is to assign carriers to each SKU **to minimize total cost**, considering both:
- The **shipping cost** (with discounts applied when thresholds are met)  
- The **manufacturing cost** of the products
  
---
## Optimization Model  

We model this problem as a **Mixed-Integer Quadratic Program (MIQP)** using **Gurobi**, with the following key elements:

- **Decision Variables**:  
  `xA[i], xB[i], xC[i]` — Number of packages of SKU *i* shipped by Carriers A, B, C (integer)  

- **Constraints**:  
  - **Fulfillment**: The sum of packages ordered across all carriers must cover demand.  
    for each SKU: total number of packages shipped to store $\geq$ (number of products sold - stock levels) / order quantities 

  - **Discount Activation**: Implemented via **indicator constraints** (using Gurobi's `addGenConstrIndicator`):  
    e.g., *if total packages of Carrier A ≥ 2500 → activate discount *  

- **Objective**:  
  Minimize: Total Shipping Cost (with discounts) + Total Manufacturing Cost

