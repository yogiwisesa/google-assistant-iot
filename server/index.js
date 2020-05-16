const express = require('express');
const bodyParser = require('body-parser');

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

  relayStatus[number] = status === 'switch' ? !relayStatus[number] : status;

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

const port = process.env.PORT;
app.listen(port, () => console.log(`Listening on port ${port}...`));