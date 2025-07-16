## Dataset
In this project, we are working with a [dataset](https://www.kaggle.com/code/amirmotefaker/supply-chain-analysis/input) from a beauty and makeup supplier.
The dataset was publicly available but contained unrealistic values, so I refined and adjusted the data to better reflect plausible industry scenarios before applying it in my optimization models.
The refined dataset is available [here](????).
### Dataset Observations and Adjustments 
- For the fields **price**, **revenue generated**,**shipping cost**, and **manufacturing cost**, the data is supposed to be in some currency.
  It’s unusual for product prices to have more than 2 decimal places since pricing reflects currency.
    - This issue is fixed by limiting these numbers to two 2 decimal places.
- The value of the **manufacturing cost** should normally not be less than the **price**.
  In fact, in most cases, the (selling) **price** should be more than **manufacturing cost**+**shipping cost** for the business to gain any profit at all.
  It is also common sense that Shipping Cost should be $\leq$ 25% of Selling Price.
    - This is fixe by re-calculating the Price (in rows in the dataset that do not make sense) as
      $(Manufacturing Cost+Shipping Cost)×(1+Markup Percentage)$
      where the typical **markup percentage** for mass-market beauty products is a ratio between [1,3].
- The field **revenue generated** is not equal to **price**$\times$**number of products sold**. This can easily be fixed.
- Excessively high **defect rates** are reported. Typical industry defect rates are <1%. Easy fix.
- **Manufacturing lead times** are ranging from 1 to 30 days, with no co-relation to the supplier. (I did not change this)
- In realistic supply chain timelines, **manufacturing lead time for cosmetics** is typically between 3–15 days and **shipping lead time** is 2–7 days for domestic days, and 7–30 days for international cases.
  - Could be fixed using ranged random generation.
- There are two fields with "lead time" header...
  - Removed one.
- Some rows have **Pass** as **inspection result** with high Defect Rates. Normally, if Defect Rate > threshold, the inspection would fail.
  - Fixed it to if Defect Rate > 2%, force Inspection Result = "Fail".
- **Transportation modes** and **shipping times** mismatch. Some products shipped by **Air** have long lead times (e.g., 30 days) even though **Air** is usually used for fast delivery.
  - Fix for **Air** shipments: **Lead Time** $\leq$ 7 days and **sea shipments**: **Lead Time** $\geq$ 15 days.
 
## Problem Statement
In this dataset, we have one carrier(out of A, B, and C) selected for each product. 
Now, let us consider the case where for each SKU(Stock Keeping Unit), all three carriers provide the **exact** same service (exact same transportation modes and exact same routes) with **different shipping costs**.
We generate these 2 other shipping costs by randomly choosing to either add or subtract 0.05 of the original carrier's shipping cost.

In this scenario, the reason the store would not immediately choose the carrier with the minimum cost of shipping for each product is that there are discounts provided by each carrier:

- Carrier A offers a **30%** discount on shipping more than 2500 ``packages''
- Carrier B offers a **50%** discount on shipping more than 2600 ``packages''
- Carrier C offers a **20%** discount on shipping more than 2200 ``packages''

For now, we will ignore the stock levels and pretend like all the sold products need to be shipped (i.e. the stock levels need to stay the same).
Note that the number of ``packages'' shipped by one carrier could be calculated as

  number of packages = number of products sold / order quantities 

In order to implement this, we store the variable **number of packages of sku $i$ shipped by each carrier** in $carrierApackages_i$, $carrierBpackages_i$, $carrierCpackages_i$, respectively.
We also store the **overall shipping cost** by each carrier in $carrierAshippingCost$, $carrierBshippingCost$, and $carrierCshippingCost$, respectively.
Then the overall profit will be calculated as follows

