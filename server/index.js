const express = require('express');
const bodyParser = require('body-parser');
const moment = require('moment');

require('dotenv').config();

const app = express();
app.use(bodyParser.json());

app.get('/', async (req, res, next) => {
  res.send({
    status: 'up'
  })
})

const relayStatus = [
  false, false, false, false, false, false, false, false
];
app.post('/relay-project/change/:number/:status', (req,res,next) => {
  const { number, status } = req.params;

  relayStatus[number - 1] = status === 'switch' ? !relayStatus[number -1] : status === 1 ? true : false;

  console.log(`Relay Project: Change Number ${number} to ${relayStatus[number - 1]}`);

  return res.send({
    status: 'ok',
    data: relayStatus
  })
});

app.get('/relay-project/status', (req,res,next) => {
  res.send({
    status: 'ok',
    data: relayStatus
  })
})

const logTemp = [];
app.get('/temperature-project/save', (req, res, next) => {
  console.log(`Temperature Project: `, req.query)
  const { humid, temp } = req.query;

  console.log(`Temperature Project: Humid: `, humid);
  console.log(`Temperature Project: Temp: `, temp);
  logTemp.push({
    dateTime: moment().format('YYYY-MMM-DD HH:mm:ss'),
    temp,
    humid
  })

  res.send({
    status: 'ok'
  })
})

app.get('/temperature-project/status', (req, res, next) => {
  res.send({
    status: 'ok',
    data: logTemp.reverse()
  })
})

const port = process.env.PORT;
app.listen(port, () => console.log(`Listening on port ${port}...`));