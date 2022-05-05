import gql from 'graphql-tag'

export const CREATE_ACCOUNT_MUTATION = gql`
  mutation ($token: String!, $public_key: String!, $name: String) {
    createAccount(token: $token, public_key: $public_key, name: $name) {
      account
    }
  }
`

export const TRANFER_FAUCET_TOKENS_MUTATION = gql`
  mutation ($token: String!, $to: String!) {
    transferFaucetTokens(token: $token, to: $to) {
      tx
    }
  }
`
