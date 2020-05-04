// 安装selenium
//      npm install selenium-webdriver
const {Builder, By, until} = require('selenium-webdriver');

let stocks = ['601111', '601288', '601398', '600036', '601988', '601766', '601318', '002594', '600660', '000002'];
const URL_TEMPLATE = 'http://finance.sina.com.cn/realstock/company/0/nc.shtml';
const reg = /(\d+)派([\d\.]+)元/;

async function 下载单个股票股息率(driver, stock) {
    if (!stock) return;
    
    let url = '';
    if (stock[0] == '6') 
        url = URL_TEMPLATE.replace('0', 'sh' + stock);
    else
        url = URL_TEMPLATE.replace('0', 'sz' + stock);
    
    await driver.get(url);
    await driver.wait(until.elementLocated(By.css('.fhsp')), 10000);
    let stockPrice = await driver.findElement(By.id('price')).getText();
    let elements = await driver.findElements(By.css('.fhsp'));
    let 分红送配表 = elements[0];
    let rows = await 分红送配表.findElements(By.tagName('tr'));
    let record = [];
    for (let row of rows) {
        let dateCell = await row.findElement(By.tagName('th'));
        let dividenCell = await row.findElement(By.tagName('td'));
        let dividen = await dividenCell.getText();
        let result = reg.exec(dividen);
        if (result) {
            let stockCount = Number.parseInt(result[1]);
            let stockDividen = Number.parseFloat(result[2]);
            record.push({
                date: await dateCell.getText(),
                dividen: stockDividen / stockCount
            });
        }
    }

    let year_dividen = 0;
    let lastYear = 0;
    for (let r of record) {
        let parts = r.date.split('-');
        let year = Number.parseInt(parts[0]);
        if (lastYear == 0) {
            lastYear = year;
            year_dividen += r.dividen;
        } else if (lastYear == year) {
            year_dividen += r.dividen;
        } else {
            break;
        }
    }
    let stockName = await driver.findElement(By.css('#stockName .c8_name')).getText();

    return {
        stock: stock,
        name: stockName,
        price: stockPrice,
        dividen: year_dividen
    };
}

(async function main() {
    let driver = await new Builder().forBrowser('firefox').build();
    console.log('股票代码,股票名称,股价,最近一年股息');
    try {
        for (let stock of stocks) {
            let record = await 下载单个股票股息率(driver, stock);
            console.log(`${stock},${record.name},${record.price},${record.dividen}`);
            // await driver.sleep(1000);
        }
    } finally {
      await driver.quit();
    }
  })();